#ifndef __LINUX_LOBBY_RENDERER_HPP__
#define __LINUX_LOBBY_RENDERER_HPP__

#include "i_lobby_renderer.hpp"
#include "i_platform_renderer.hpp"
#include "lobby_network_packet.hpp"
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
    void render_big_text(int x, int y, const std::string&, Color);
    void render_small_text(int x, int y, const std::string&, Color);
    char comment_list[COMMENTLISTNUM][COMMENTSIZE];
    int comment_list_index = 0;
    char comment_user_list[COMMENTLISTNUM][COMMENTSIZE];
    int comment_user_list_index = 0;

  public:
    LinuxLobbyRenderer(const InputWindowRenderer&, const TextRenderer&, IPlatformRenderer*);
    void render_set_nickname(const std::string&) override;
    void render_entrance() override;
    void render_entrance_choice(Entrance entrance) override;
    void render_create_room() override;
    void render_room(const std::string& room_name, const std::string& host_name, bool is_server) override;
    void render_room_clients(std::unordered_map<std::string, std::string>&) override;
    void render_lobby() override;
    void render_lobby_rooms(std::vector<std::pair<std::string, std::string>>&, int) override;
    void render_user_id_input() override;
    void render_clear() override;
    void render_my_chat(const char* comment, const std::string& id) override;
    void render_other_user_chat(const char* comment, const std::string& id) override;
};

#endif
