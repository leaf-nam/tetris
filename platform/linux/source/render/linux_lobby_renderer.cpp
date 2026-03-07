#include "render/linux_lobby_renderer.hpp"

#include "entrance.hpp"
#include "i_platform_renderer.hpp"

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

void LinuxLobbyRenderer::render_small_text(int x, int y, const string& str)
{
    text_renderer.print_small_string({x, y}, str, Color::CYAN);
}

void LinuxLobbyRenderer::render_set_nickname(const string& cur_nickname)
{
    render_clear();
    render_big_text(20, 7, "SET NICKNAME");
    render_small_text(27, 18, "Current Nickname : [ " + cur_nickname + " ] ");
    render_input_window(27, 20, "Type Your Nickname.[length : 1 ~ 8]");
}
void LinuxLobbyRenderer::render_entrance()
{
    render_clear();
    render_big_text(12, 7, "ENTRANCE");
}

void LinuxLobbyRenderer::render_entrance_choice(Entrance entrance)
{
    const int X = 25, Y = 18;
    string text;
    Color text_color;
    platform_renderer->hide_cursor();

    for (int i = 0; i < ENTRANCE_COUNT; ++i) {
        platform_renderer->set_cursor(X, Y + i * 2);

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
    render_big_text(10, 7, "CRAETE ROOM");
    render_input_window(27, 20, "Type New Room Name.[length : 1 ~ 8]");
}

void LinuxLobbyRenderer::render_lobby(const string& room_name, const string& host_name)
{
    render_clear();
    render_big_text(14, 7, "LOBBY");
    render_small_text(27, 20, "ROOM NAME : [ " + room_name + " ] ");
    render_small_text(27, 22, "HOST : [ " + host_name + " ] ");
}

void LinuxLobbyRenderer::render_lobby_clients(vector<string>& clients)
{
    for (auto client : clients) {
    }
}

void LinuxLobbyRenderer::render_server_view_room(char* server_id,
                                                 unordered_map<string, string> client_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("%s\n", server_id);
    for (const auto& [key, value] : client_ip_address)
        printf("%s\n", key.c_str());
    printf("Press Any key + enter for start game or Press q + enter for get out from room\n");
}

void LinuxLobbyRenderer::render_user_id_input()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("\e[?25l");
    printf("Please enter USER ID(8): ");
    fflush(stdout);
}

void LinuxLobbyRenderer::render_client_view_room(char* server_id,
                                                 unordered_map<string, string> client_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("ROOM HOST: %s\n", server_id);
    for (const auto& [key, value] : client_ip_address)
        printf("%s\n", key.c_str());
    printf("Press Any key + enter for get out room\n");
}

void LinuxLobbyRenderer::render_view_enter_room(unordered_map<string, string> server_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    for (const auto& [key, value] : server_ip_address)
        printf("%s : %s\n", value.c_str(), key.c_str());
    printf("Get Room constantly...(exit q + enter)\n");
}

void LinuxLobbyRenderer::render_clear() { platform_renderer->clear(); }

void LinuxLobbyRenderer::render_select()
{
    printf("\033[37m"); // foreground white
    fflush(stdout);
    printf("\033[%d;%dH", 1, 1);
    printf("1. open room, 2. enter room\n");
    fflush(stdout);
}
