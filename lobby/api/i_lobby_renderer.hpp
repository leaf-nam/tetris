#ifndef __I_LOBBY_RENDERER_HPP__
#define __I_LOBBY_RENDERER_HPP__

#include <unordered_map>
#include <string>

class ILobbyRenderer
{
  public:
    virtual void render_server_view_room(char* server_id,
                                         std::unordered_map<std::string, std::string> client_ip_address) = 0;
    virtual void render_user_id_input() = 0;
    virtual void render_client_view_room(char* server_id,
                                         std::unordered_map<std::string, std::string> client_ip_address) = 0;
    virtual void render_view_enter_room(std::unordered_map<std::string, std::string> server_ip_address) = 0;
    virtual void render_clear() = 0;
    virtual void render_select() = 0;
    virtual ~ILobbyRenderer() = default;
};

#endif
