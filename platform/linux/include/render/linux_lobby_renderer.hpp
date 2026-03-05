#ifndef __LINUX_LOBBY_RENDERER_HPP__
#define __LINUX_LOBBY_RENDERER_HPP__

#include "i_lobby_renderer.hpp"
#include "i_platform_renderer.hpp"
#include "lobby.hpp"
#include "render/text_renderer.hpp"

class LinuxLobbyRenderer : public ILobbyRenderer
{
  private:
    TextRenderer* text_renderer;
    IPlatformRenderer* platform_renderer;

  public:
    void render_entrance() override;
    void render_entrance_menu(EntranceMenu) override;
    void render_set_nickname() override;
    void render_create_room() override;
    void render_enter_room() override;

    void render_server_view_room(
        char* server_id, std::unordered_map<std::string, std::string> client_ip_address) override;
    void render_user_id_input() override;
    void render_client_view_room(
        char* server_id, std::unordered_map<std::string, std::string> client_ip_address) override;
    void
    render_view_enter_room(std::unordered_map<std::string, std::string> server_ip_address) override;
    void render_clear() override;
    void render_select() override;
};

#endif
