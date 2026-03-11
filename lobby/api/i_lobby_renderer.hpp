#ifndef __I_LOBBY_RENDERER_HPP__
#define __I_LOBBY_RENDERER_HPP__

#include "entrance.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#define COMMENTLISTNUM 10

class ILobbyRenderer
{
  public:
    virtual void render_set_nickname(const std::string&) = 0;
    virtual void render_entrance() = 0;
    virtual void render_entrance_choice(Entrance) = 0;
    virtual void render_create_room() = 0;
    virtual void render_room(const std::string& room_name, const std::string& host_name, bool is_server) = 0;
    virtual void render_room_clients(std::unordered_map<std::string, std::string>&) = 0;
    virtual void render_lobby() = 0;
    virtual void render_lobby_rooms(std::vector<std::pair<std::string, std::string>>&, int) = 0;
    virtual void render_user_id_input() = 0;
    virtual void render_clear() = 0;
    virtual void render_my_chat(const char* comment, const std::string& id) = 0;
    virtual void render_other_user_chat(const char* comment, const std::string& id) = 0;
    virtual void render_current_chat() = 0;
    virtual void render_clear_chat() = 0;
    virtual ~ILobbyRenderer() = default;
};

#endif
