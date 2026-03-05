#include "engine/solo_engine.hpp"
#include "i_input_handler.hpp"
#include "i_renderer.hpp"
#include "setting.hpp"
#include "setting_storage.hpp"

#include <emscripten/emscripten.h>

EM_JS(void, js_render_background, (), { Module.webRenderer?.render_background(); });
EM_JS(void, js_render_timer, (int sec), { Module.webRenderer?.render_timer(sec); });
EM_JS(void, js_render_score, (int score), { Module.webRenderer?.render_score(score); });
EM_JS(void, js_render_level, (int level), { Module.webRenderer?.render_level(level); });
EM_JS(void, js_render_clear, (), { Module.webRenderer?.render_clear(); });
EM_JS(void, js_render_game_over, (), { Module.webRenderer?.render_game_over(); });
EM_JS(void, js_render_board_from_heap, (), { Module.webRenderer?.render_board_from_heap(); });

static int g_last_key = 0;
static int g_board_flat[BOARD_ROW * BOARD_COL];
static int g_active_shape[MINO_SIZE * MINO_SIZE];
static int g_meta[4]; // [row, col, rot, type]

extern "C" EMSCRIPTEN_KEEPALIVE void web_set_key(int key) { g_last_key = key; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_board_ptr() { return g_board_flat; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_active_shape_ptr() { return g_active_shape; }
extern "C" EMSCRIPTEN_KEEPALIVE int* get_meta_ptr() { return g_meta; }

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
    void export_render_state(const Board& board)
    {
        const BoardT& bb = board.get_board();
        int k = 0;
        for (int r = 0; r < BOARD_ROW; r++)
            for (int c = 0; c < BOARD_COL; c++)
                g_board_flat[k++] = bb[r][c];

        Board& mutable_board = const_cast<Board&>(board);
        auto [ar, ac] = mutable_board.get_active_mino_pos();
        const int rot = mutable_board.get_active_mino_rotation();
        const int type = mutable_board.get_active_mino_type();
        g_meta[0] = ar;
        g_meta[1] = ac;
        g_meta[2] = rot;
        g_meta[3] = type;

        const Mino& m = mutable_board.get_active_mino().get_shape(rot);
        int idx = 0;
        for (int r = 0; r < MINO_SIZE; r++)
            for (int c = 0; c < MINO_SIZE; c++)
                g_active_shape[idx++] = m[r][c];
    }

  public:
    void render_background() override { js_render_background(); }

    void render_board(const Board& board, const Tetromino&) override
    {
        export_render_state(board);
        js_render_board_from_heap();
    }

    void render_timer(int sec) override { js_render_timer(sec); }
    void render_next_block(const int*) override {}
    void render_hold(const Tetromino&) override {}
    void render_score(int score) override { js_render_score(score); }
    void render_other_board(Packet&) override {}
    void render_game_over() override { js_render_game_over(); }
    void render_other_game_over(Packet&) override {}
    void render_win() override {}
    void render_other_win(Packet&) override {}
    void render_level(int level) override { js_render_level(level); }
    void render_clear() override { js_render_clear(); }
};

static IInputHandler* input = nullptr;
static IRenderer* renderer = nullptr;
static SoloEngine* engine = nullptr;
static Setting* setting = nullptr;
static bool loop_running = false;

static void destroy_game()
{
    if (engine != nullptr) {
        engine->finish();
        delete engine;
        engine = nullptr;
    }
    delete input;
    delete renderer;
    delete setting;
    input = nullptr;
    renderer = nullptr;
    setting = nullptr;
}

static void init_game()
{
    destroy_game();

    SettingStorage& storage = SettingStorage::getInstance();
    storage.initialize("settings.txt");
    setting = new Setting(storage.load());

    input = new WebInput();
    renderer = new WebRenderer();
    engine = new SoloEngine(setting, input, renderer);
    engine->init(false);
}

static void frame()
{
    if (engine == nullptr) return;

    if (!engine->run(false)) {
        renderer->render_game_over();
        emscripten_cancel_main_loop();
        loop_running = false;
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE void game_start()
{
    if (engine == nullptr) init_game();
    if (loop_running) return;

    emscripten_set_main_loop(frame, 0, 1);
    loop_running = true;
}

extern "C" EMSCRIPTEN_KEEPALIVE void game_reset()
{
    if (loop_running) {
        emscripten_cancel_main_loop();
        loop_running = false;
    }
    init_game();
    game_start();
}

int main()
{
    init_game();
    game_start();
    return 0;
}
