#ifndef __I_IP_RESOLVER_INPUT_HANDLER_HPP__
#define __I_IP_RESOLVER_INPUT_HANDLER_HPP__

class IIpResolverInputHandler
{
  public:
    // is_blocking == 1 => true, 0 => false
    virtual int scan(char* buf, int is_blocking) = 0;
    virtual int scan(int* buf, int is_blocking) = 0;
    ~IIpResolverInputHandler() = default;
};

#endif