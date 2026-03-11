#include "app_state.hpp"
#include "engine/solo_engine.hpp"
#include "i_input_handler.hpp"
#include "i_lobby_input_handler.hpp"
#include "i_lobby_renderer.hpp"
#include "i_renderer.hpp"
#include "lobby.hpp"
#include "lobby_network/web_lobby_network.hpp"
#include "setting.hpp"
#include "setting_storage.hpp"

#include <emscripten/emscripten.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

EM_JS(void, js_render_background, (), { Module.webRenderer?.render_background(); });
EM_JS(void, js_render_timer, (int sec), { Module.webRenderer?.render_timer(sec); });
EM_JS(void, js_render_score, (int score), { Module.webRenderer?.render_score(score); });
EM_JS(void, js_render_level, (int level), { Module.webRenderer?.render_level(level); });
EM_JS(void, js_render_clear, (), { Module.webRenderer?.render_clear(); });
EM_JS(void, js_render_game_over, (), { Module.webRenderer?.render_game_over(); });
EM_JS(void, js_render_board_from_heap, (), { Module.webRenderer?.render_board_from_heap(); });
EM_JS(void, js_render_hold_from_heap, (), { Module.webRenderer?.render_hold_from_heap(); });
EM_JS(void, js_render_next_from_heap, (), { Module.webRenderer?.render_next_from_heap(); });

EM_JS(void, js_lobby_begin, (), { Module.webLobbyUi?.begin(); });
EM_JS(void, js_lobby_finish, (int state), { Module.webLobbyUi?.finish(state); });
EM_JS(void, js_lobby_render_set_nickname, (const char* nickname), {
    Module.webLobbyUi?.renderSetNickname(UTF8ToString(nickname));
});
EM_JS(void, js_lobby_render_entrance, (), { Module.webLobbyUi?.renderEntrance(); });
EM_JS(void, js_lobby_render_entrance_choice, (int entrance), {
    Module.webLobbyUi?.renderEntranceChoice(entrance);
});
EM_JS(void, js_lobby_render_create_room, (), { Module.webLobbyUi?.renderCreateRoom(); });
EM_JS(void, js_lobby_render_room, (const char* room_name, const char* host_name, int is_server), {
    Module.webLobbyUi?.renderRoom(UTF8ToString(room_name), UTF8ToString(host_name), is_server !== 0);
});
EM_JS(void, js_lobby_render_room_clients, (const char* clients), {
    Module.webLobbyUi?.renderRoomClients(UTF8ToString(clients));
});
EM_JS(void, js_lobby_render_lobby, (), { Module.webLobbyUi?.renderLobby(); });
EM_JS(void, js_lobby_render_lobby_rooms, (const char* rooms, int selected), {
    Module.webLobbyUi?.renderLobbyRooms(UTF8ToString(rooms), selected);
});
EM_JS(void, js_lobby_render_user_id_input, (), { Module.webLobbyUi?.renderUserIdInput(); });
EM_JS(void, js_lobby_render_clear, (), { Module.webLobbyUi?.renderClear(); });

EM_ASYNC_JS(int, js_lobby_scan_key, (), {
    await new Promise((resolve) => setTimeout(resolve, 16));
    return Module.webLobbyUi ? (Module.webLobbyUi.consumeKey() | 0) : 0;
});
EM_ASYNC_JS(int, js_lobby_scan_text, (char* buf_ptr, int buf_len, int is_blocking), {
    while (true) {
        const ui = Module.webLobbyUi;
        const value = ui ? ui.consumeText() : "";
        if (value) {
            stringToUTF8(String(value).slice(0, Math.max(0, buf_len - 1)), buf_ptr, buf_len);
            return 1;
        }
        if (!is_blocking) {
            return 0;
        }
        await new Promise((resolve) => setTimeout(resolve, 16));
    }
});
EM_ASYNC_JS(int, js_lobby_scan_int, (int* buf_ptr, int is_blocking), {
    while (true) {
        const ui = Module.webLobbyUi;
        const value = ui ? ui.consumeText() : "";
        if (value) {
            const parsed = Number.parseInt(value, 10);
            if (!Number.isNaN(parsed)) {
                Module.HEAP32[buf_ptr >> 2] = parsed | 0;
                return 1;
            }
        }
        if (!is_blocking) {
            return 0;
        }
        await new Promise((resolve) => setTimeout(resolve, 16));
    }
});

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

void ensure_setting_loaded()
{
    if (g_setting != nullptr) return;

    SettingStorage& storage = SettingStorage::getInstance();
    storage.initialize("settings.txt");
    g_setting = std::make_unique<Setting>(storage.load());
}

std::string join_client_ids(std::unordered_map<std::string, std::string>& clients)
{
    std::string result;
    for (const auto& [id, ip] : clients) {
        (void)ip;
        if (!result.empty()) result.push_back('\n');
        result.append(id);
    }
    return result;
}

std::string join_rooms(std::vector<std::pair<std::string, std::string>>& rooms)
{
    std::string result;
    for (const auto& [room_name, ip] : rooms) {
        if (!result.empty()) result.push_back('\n');
        result.append(room_name);
        result.push_back('|');
        result.append(ip);
    }
    return result;
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

class WebLobbyInputHandler final : public ILobbyInputHandler {
  public:
    int scan() override { return js_lobby_scan_key(); }

    int scan(char* buf, int buf_len, int is_blocking) override
    {
        return js_lobby_scan_text(buf, buf_len, is_blocking);
    }

    int scan(int* buf, int is_blocking) override
    {
        return js_lobby_scan_int(buf, is_blocking);
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

class WebLobbyRenderer final : public ILobbyRenderer {
  public:
    void render_set_nickname(const std::string& nickname) override
    {
        js_lobby_render_set_nickname(nickname.c_str());
    }

    void render_entrance() override { js_lobby_render_entrance(); }

    void render_entrance_choice(Entrance entrance) override
    {
        js_lobby_render_entrance_choice(static_cast<int>(entrance));
    }

    void render_create_room() override { js_lobby_render_create_room(); }

    void render_room(const std::string& room_name, const std::string& host_name, bool is_server) override
    {
        js_lobby_render_room(room_name.c_str(), host_name.c_str(), is_server ? 1 : 0);
    }

    void render_room_clients(std::unordered_map<std::string, std::string>& clients) override
    {
        const std::string joined = join_client_ids(clients);
        js_lobby_render_room_clients(joined.c_str());
    }

    void render_lobby() override { js_lobby_render_lobby(); }

    void render_lobby_rooms(std::vector<std::pair<std::string, std::string>>& rooms, int selected) override
    {
        const std::string joined = join_rooms(rooms);
        js_lobby_render_lobby_rooms(joined.c_str(), selected);
    }

    void render_user_id_input() override { js_lobby_render_user_id_input(); }
    void render_clear() override { js_lobby_render_clear(); }
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
}

static void init_game()
{
    destroy_game();
    ensure_setting_loaded();

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

extern "C" EMSCRIPTEN_KEEPALIVE void game_pause() { pause_loop(); }

extern "C" EMSCRIPTEN_KEEPALIVE int web_run_lobby()
{
    ensure_setting_loaded();
    js_lobby_begin();

    WebLobbyNetwork network;
    WebLobbyRenderer renderer;
    WebLobbyInputHandler input;
    Lobby lobby(g_setting.get(), &network, &renderer, &input);

    const AppState result = lobby.start();
    js_lobby_finish(static_cast<int>(result));
    return static_cast<int>(result);
}

int main()
{
    init_game();
    ensure_loop_started();
    return 0;
}
