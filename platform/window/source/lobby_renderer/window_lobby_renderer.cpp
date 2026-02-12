#include "lobby_renderer/window_lobby_renderer.hpp"

void WindowLobbyRenderer::render_server_view_room(
    char* server_id, std::unordered_map<std::string, std::string> client_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("%s\n", server_id);
    for (const auto& [key, value] : client_ip_address)
        printf("%s\n", key.c_str());
    printf("Press Any key + enter for start game or Press q + enter for get out from room\n");
}

void WindowLobbyRenderer::render_user_id_input()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("\e[?25l");
    printf("Please enter USER ID(8): ");
    fflush(stdout);
}

void WindowLobbyRenderer::render_client_view_room(
    char* server_id, std::unordered_map<std::string, std::string> client_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    printf("ROOM HOST: %s\n", server_id);
    for (const auto& [key, value] : client_ip_address)
        printf("%s\n", value.c_str());
    printf("Press Any key + enter for get out room\n");
}

void WindowLobbyRenderer::render_view_enter_room(
    std::unordered_map<std::string, std::string> server_ip_address)
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
    for (const auto& [key, value] : server_ip_address)
        printf("%s : %s\n", value.c_str(), key.c_str());
    printf("Get Room constantly...(exit q + enter)\n");
}

void WindowLobbyRenderer::render_clear()
{
    printf("\033[2J\033[1;1H");
    fflush(stdout);
}

void WindowLobbyRenderer::render_select()
{ 
    printf("\033[37m"); // foreground white
    fflush(stdout);
    printf("\033[%d;%dH", 1, 1);
    printf("1. open room, 2. enter room\n");
    fflush(stdout);
}

