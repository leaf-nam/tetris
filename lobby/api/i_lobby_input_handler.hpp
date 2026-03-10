#ifndef __I_LOBBY_INPUT_HANDLER_HPP__
#define __I_LOBBY_INPUT_HANDLER_HPP__

#include <i_input_handler.hpp>

class ILobbyInputHandler
{
  public:
    // is_blocking == 1 => true, 0 => false
    virtual int scan() = 0;
    virtual int scan(char* buf, int buf_len, int is_blocking) = 0;
    virtual int scan(int* buf, int is_blocking) = 0;
    virtual ~ILobbyInputHandler() = default;
};

#endif
