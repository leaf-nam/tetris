#include "render/linux_lobby_renderer.hpp"

LinuxLobbyRenderer::LinuxLobbyRenderer(const InputWindowRenderer& input_render,
                                       const TextRenderer& text_render)
    : input_window_renderer(input_render), text_renderer(text_render)
{
}

void LinuxLobbyRenderer::render_input_window(int x, int y, const std::string& str)
{
    input_window_renderer.render_input_window({x, y}, str);
}

void LinuxLobbyRenderer::render_big_text(int x, int y, const std::string& str)
{
    text_renderer.print_big_string({x, y}, str);
}

void LinuxLobbyRenderer::render_small_text(int x, int y, const std::string& str)
{
    text_renderer.print_small_string({x, y}, str, Color::CYAN);
}

void LinuxLobbyRenderer::render_server_view_room(
    char* server_id, std::unordered_map<std::string, std::string> client_ip_address)
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

void LinuxLobbyRenderer::render_client_view_room(
    char* server_id, std::unordered_map<std::string, std::string> client_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("ROOM HOST: %s\n", server_id);
    for (const auto& [key, value] : client_ip_address)
        printf("%s\n", key.c_str());
    printf("Press Any key + enter for get out room\n");
}

void LinuxLobbyRenderer::render_view_enter_room(
    std::unordered_map<std::string, std::string> server_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    for (const auto& [key, value] : server_ip_address)
        printf("%s : %s\n", value.c_str(), key.c_str());
    printf("Get Room constantly...(exit q + enter)\n");
}

void LinuxLobbyRenderer::render_clear()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void LinuxLobbyRenderer::render_select()
{
    printf("\033[37m"); // foreground white
    fflush(stdout);
    printf("\033[%d;%dH", 1, 1);
    printf("1. open room, 2. enter room\n");
    fflush(stdout);
}
