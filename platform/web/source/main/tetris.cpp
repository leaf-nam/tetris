#include "engine/solo_engine.hpp"
#include "i_input_handler.hpp"
#include "i_renderer.hpp"
#include "setting.hpp"
#include "setting_storage.hpp"

#include <emscripten/emscripten.h>
#include <memory>

EM_JS(void, js_render_background, (), { Module.webRenderer?.render_background(); });
EM_JS(void, js_render_timer, (int sec), { Module.webRenderer?.render_timer(sec); });
EM_JS(void, js_render_score, (int score), { Module.webRenderer?.render_score(score); });
EM_JS(void, js_render_level, (int level), { Module.webRenderer?.render_level(level); });
EM_JS(void, js_render_clear, (), { Module.webRenderer?.render_clear(); });
EM_JS(void, js_render_game_over, (), { Module.webRenderer?.render_game_over(); });
EM_JS(void, js_render_board_from_heap, (), { Module.webRenderer?.render_board_from_heap(); });
EM_JS(void, js_render_hold_from_heap, (), { Module.webRenderer?.render_hold_from_heap(); });
EM_JS(void, js_render_next_from_heap, (), { Module.webRenderer?.render_next_from_heap(); });

namespace {
constexpr bool kIsServer = false;
constexpr int kShapeCellCount = MINO_SIZE * MINO_SIZE;
constexpr int kBoardMetaSize = 5;
constexpr int kHoldMetaSize = 1;
constexpr int kNextCount = 3;
enum MetaIndex : int
{
    META_ROW = 0,
    META_COL = 1,
    META_ROT = 2,
    META_TYPE = 3,
    META_SHADOW_ON = 4
};

static int g_last_key = 0;
static int g_board_flat[BOARD_ROW * BOARD_COL];
static int g_active_shape[kShapeCellCount];
static int g_board_meta[kBoardMetaSize];
static int g_hold_shape[kShapeCellCount];
static int g_hold_meta[kHoldMetaSize];
static int g_next_shapes[kNextCount * kShapeCellCount];
static int g_next_types[kNextCount];

static std::unique_ptr<IInputHandler> g_input;
static std::unique_ptr<IRenderer> g_renderer;
static std::unique_ptr<SoloEngine> g_engine;
static std::unique_ptr<Setting> g_setting;
static bool g_loop_initialized = false;
static bool g_loop_running = false;

void clear_shape_buffer(int* buffer)
{
    for (int i = 0; i < kShapeCellCount; ++i)
        buffer[i] = 0;
}

void export_shape(const Mino& mino, int* buffer)
{
    int idx = 0;
    for (int r = 0; r < MINO_SIZE; ++r)
        for (int c = 0; c < MINO_SIZE; ++c)
            buffer[idx++] = mino[r][c];
}

void export_tetromino_preview(const Tetromino& tetromino, int* shape_buffer, int& type_out)
{
    type_out = tetromino.get_mino_type();
    clear_shape_buffer(shape_buffer);

    if (type_out < 0 || type_out >= MINO_KIND)
        return;

    export_shape(tetromino.get_shape(), shape_buffer);
}
} // namespace

static void frame();

extern "C" EMSCRIPTEN_KEEPALIVE void web_set_key(int key) { g_last_key = key; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_board_ptr() { return g_board_flat; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_active_shape_ptr() { return g_active_shape; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_meta_ptr() { return g_board_meta; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_hold_shape_ptr() { return g_hold_shape; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_hold_meta_ptr() { return g_hold_meta; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_next_shapes_ptr() { return g_next_shapes; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_next_types_ptr() { return g_next_types; }

class WebInput final : public IInputHandler {
  public:
    int scan() override
    {
        const int key = g_last_key;
        g_last_key = 0;
        return key;
    }
};

class WebRenderer final : public IRenderer {
  private:
    void export_render_state(Board& board)
    {
        const BoardT& bb = board.get_board();
        int k = 0;
        for (int r = 0; r < BOARD_ROW; r++)
            for (int c = 0; c < BOARD_COL; c++)
                g_board_flat[k++] = bb[r][c];

        auto [ar, ac] = board.get_active_mino_pos();
        const int rot = board.get_active_mino_rotation();
        const int type = board.get_active_mino_type();
        g_board_meta[META_ROW] = ar;
        g_board_meta[META_COL] = ac;
        g_board_meta[META_ROT] = rot;
        g_board_meta[META_TYPE] = type;
        g_board_meta[META_SHADOW_ON] =
            (g_setting != nullptr && g_setting->shadow_on) ? 1 : 0;

        clear_shape_buffer(g_active_shape);
        if (type >= 0 && type < MINO_KIND)
            export_shape(board.get_active_mino().get_shape(rot), g_active_shape);
    }

  public:
    void render_background() override { js_render_background(); }

    void render_board(const Board& board, const Tetromino&) override
    {
        Board& mutable_board = const_cast<Board&>(board);
        export_render_state(mutable_board);
        js_render_board_from_heap();
    }

    void render_timer(int sec) override { js_render_timer(sec); }

    void render_next_block(const int* tetrominoArray) override
    {
        for (int i = 0; i < kNextCount; ++i) {
            Tetromino next_mino;
            next_mino.init_mino(tetrominoArray[i]);
            export_tetromino_preview(next_mino, &g_next_shapes[i * kShapeCellCount], g_next_types[i]);
        }
        js_render_next_from_heap();
    }

    void render_hold(const Tetromino& tetromino) override
    {
        export_tetromino_preview(tetromino, g_hold_shape, g_hold_meta[0]);
        js_render_hold_from_heap();
    }

    void render_score(int score) override { js_render_score(score); }
    void render_other_board(Packet&) override {}
    void render_game_over() override { js_render_game_over(); }
    void render_other_game_over(Packet&) override {}
    void render_win() override {}
    void render_other_win(Packet&) override {}
    void render_level(int level) override { js_render_level(level); }
    void render_clear() override { js_render_clear(); }
};

static void pause_loop()
{
    if (!g_loop_initialized || !g_loop_running) return;

    emscripten_pause_main_loop();
    g_loop_running = false;
}

static void ensure_loop_started()
{
    if (!g_loop_initialized) {
        g_loop_initialized = true;
        g_loop_running = true;
        emscripten_set_main_loop(frame, 0, 1);
        return;
    }

    if (!g_loop_running) {
        g_loop_running = true;
        emscripten_resume_main_loop();
    }
}

static void destroy_game()
{
    if (g_engine != nullptr) {
        g_engine->finish();
        g_engine.reset();
    }
    g_input.reset();
    g_renderer.reset();
    g_setting.reset();
}

static void init_game()
{
    destroy_game();

    SettingStorage& storage = SettingStorage::getInstance();
    storage.initialize("settings.txt");
    g_setting = std::make_unique<Setting>(storage.load());

    g_input = std::make_unique<WebInput>();
    g_renderer = std::make_unique<WebRenderer>();
    g_engine = std::make_unique<SoloEngine>(g_setting.get(), g_input.get(), g_renderer.get());
    g_engine->init(kIsServer);
}

static void frame()
{
    if (!g_loop_running || g_engine == nullptr) return;

    if (!g_engine->run(kIsServer)) {
        if (g_renderer != nullptr) g_renderer->render_game_over();
        pause_loop();
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE void game_start()
{
    if (g_engine == nullptr) init_game();
    ensure_loop_started();
}

extern "C" EMSCRIPTEN_KEEPALIVE void game_reset()
{
    init_game();
    ensure_loop_started();
}

int main()
{
    init_game();
    ensure_loop_started();
    return 0;
}
