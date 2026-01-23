#ifndef __WINDOW_IP_RESOLVER_INPUT_HANDLER_HPP__
#define __WINDOW_IP_RESOLVER_INPUT_HANDLER_HPP__

#include "i_ip_resolver_input_handler.hpp"

class WindowIpResolverInputHandler : public IIpResolverInputHandler
{
  public:
    int scan(char* buf, int is_blocking) override;
    int scan(int* buf, int is_blocking) override;
};

#endif