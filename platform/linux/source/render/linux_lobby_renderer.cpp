#include "entrance.hpp"
#include "i_platform_renderer.hpp"
#include "render/linux_lobby_renderer.hpp"

#include <string>
#include <vector>

using namespace std;

LinuxLobbyRenderer::LinuxLobbyRenderer(const InputWindowRenderer& input_render,
                                       const TextRenderer& text_render,
                                       IPlatformRenderer* i_platform_renderer)
    : input_window_renderer(input_render), text_renderer(text_render),
      platform_renderer(i_platform_renderer)
{
}

void LinuxLobbyRenderer::render_input_window(int x, int y, const string& str)
{
    input_window_renderer.render_input_window({x, y}, str);
}

void LinuxLobbyRenderer::render_big_text(int x, int y, const string& str)
{
    text_renderer.print_big_string({x, y}, str);
}

void LinuxLobbyRenderer::render_big_text(int x, int y, const string& str, Color color)
{
    text_renderer.print_big_string({x, y}, str, color);
}

void LinuxLobbyRenderer::render_small_text(int x, int y, const string& str, Color color)
{
    text_renderer.print_small_string({x, y}, str, color);
}

void LinuxLobbyRenderer::render_set_nickname(const string& cur_nickname)
{
    render_clear();
    render_big_text(20, 7, "SET NICKNAME", Color::CYAN);
    render_small_text(27, 18, "Current Nickname : [ " + cur_nickname + " ] ", Color::GREEN);
    render_input_window(27, 20, "Type Your Nickname.[length : 1 ~ 8]");
}
void LinuxLobbyRenderer::render_entrance()
{
    render_clear();
    render_big_text(22, 7, "ENTRANCE", Color::ORANGE);
}

void LinuxLobbyRenderer::render_entrance_choice(Entrance entrance)
{
    const int X = 25, Y = 18;
    string text;
    Color text_color;
    platform_renderer->hide_cursor();

    for (int i = 0; i < ENTRANCE_COUNT; ++i) {
        platform_renderer->set_cursor(X, Y + (i + 1) * 2);

        int title_num = static_cast<int>(entrance);
        Color text_color = (title_num == i) ? Color::CYAN : Color::COMMENT;
        string text = (title_num == i) ? " > " : "   ";

        text += ENTRANCE_TITLE[i];

        platform_renderer->print_s(text, text_color);
    }
}

void LinuxLobbyRenderer::render_create_room()
{
    render_clear();
    render_big_text(20, 7, "CRAETE ROOM", Color::PURPLE);
    render_input_window(27, 20, "Type New Room Name.[length : 1 ~ 8]");
}

void LinuxLobbyRenderer::render_room(const string& room_name, const string& host_name, bool is_server)
{
    render_clear();
    render_big_text(15, 7, "WAITNG ROOM", Color::GREEN);
    if (is_server) {
        render_small_text(27, 14, " > Start Game : Press [ Space ]", Color::ORANGE);
        render_small_text(27, 16, " > Delete Room : Press [ ESC ]", Color::ORANGE);
    } else {
        render_small_text(27, 14, " > Out Room : Press [ ESC ]", Color::ORANGE);
    }
    render_small_text(27, 20, "ROOM NAME : [ " + room_name + " ] ", Color::CYAN);
    render_small_text(27, 22, "HOST : [ " + host_name + " ] ", Color::CYAN);
}

void LinuxLobbyRenderer::render_room_clients(unordered_map<string, string>& client_ip_address)
{
    int X = 27, Y = 24, dy = 0;
    render_small_text(X, Y, "=====GUESTS=====", Color::GREEN);

    platform_renderer->hide_cursor();
    for (const auto& [key, value] : client_ip_address) {
        dy++;
        platform_renderer->set_cursor(X, Y + dy * 2);

        string s = " - ";
        s += key;

        platform_renderer->print_s(s, Color::GREEN);
    }
}

void LinuxLobbyRenderer::render_lobby()
{
    render_clear();
    render_big_text(24, 7, "LOBBY", Color::ORANGE);
    render_small_text(27, 14, " > Join Room : Press [ Space ]", Color::CYAN);
    render_small_text(27, 16, " > Exit to Menu : Pres [ ESC ]", Color::CYAN);
}

void LinuxLobbyRenderer::render_lobby_rooms(vector<pair<string, string>>& rooms, int select)
{
    int X = 27, Y = 20, dy = 0;
    render_small_text(X, Y - 2, "=====ROOMS=====", Color::ORANGE);

    platform_renderer->hide_cursor();
    for (int i = 0; i < rooms.size(); ++i) {
        string room = rooms[i].first;
        platform_renderer->set_cursor(X, Y + i * 2);

        string s = i == select ? " > [ " + room + " ]" : " - " + room + "    ";

        platform_renderer->print_s(s, Color::ORANGE);
    }
}

void LinuxLobbyRenderer::render_user_id_input()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("\e[?25l");
    printf("Please enter USER ID(8): ");
    fflush(stdout);
}

void LinuxLobbyRenderer::render_clear() { platform_renderer->clear(); }
