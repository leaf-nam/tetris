#ifndef __WINDOW_IP_RESOLVER_RENDERER_HPP__
#define __WINDOW_IP_RESOLVER_RENDERER_HPP__

#include "i_ip_resolver_renderer.hpp"

#include <unordered_map>
#include <string>

class WindowIpResolverRenderer : public IIpResolverRenderer
{
  public:
    void render_server_view_room(char* server_id,
                                 std::unordered_map<std::string, std::string> client_ip_address) override;
    void render_user_id_input() override;
    void render_client_view_room(char* server_id,
                                 std::unordered_map<std::string, std::string> client_ip_address) override;
    void render_view_enter_room(std::unordered_map<std::string, std::string> server_ip_address) override;
    void render_clear() override;
    void render_select() override;
};

#endif