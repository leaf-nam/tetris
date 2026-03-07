#ifndef __LINUX_LOBBY_RENDERER_HPP__
#define __LINUX_LOBBY_RENDERER_HPP__

#include "i_lobby_renderer.hpp"
#include "i_platform_renderer.hpp"
#include "render/input_window_renderer.hpp"
#include "render/text_renderer.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class LinuxLobbyRenderer : public ILobbyRenderer
{
  private:
    InputWindowRenderer input_window_renderer;
    TextRenderer text_renderer;
    IPlatformRenderer* platform_renderer;
    void render_input_window(int x, int y, const std::string&);
    void render_big_text(int x, int y, const std::string&);
    void render_small_text(int x, int y, const std::string&);

  public:
    LinuxLobbyRenderer(const InputWindowRenderer&, const TextRenderer&, IPlatformRenderer*);
    void render_set_nickname(const std::string&) override;
    void render_entrance() override;
    void render_entrance_choice(Entrance entrance) override;
    void render_create_room() override;
    void render_lobby(const std::string& room_name, const std::string& host_name) override;
    void render_lobby_clients(std::vector<std::string>&) override;
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
