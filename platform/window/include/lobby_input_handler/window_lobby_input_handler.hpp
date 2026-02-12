#ifndef __WINDOW_LOBBY_INPUT_HANDLER_HPP__
#define __WINDOW_LOBBY_INPUT_HANDLER_HPP__

#include "i_lobby_input_handler.hpp"

class WindowLobbyInputHandler : public ILobbyInputHandler
{
  public:
    int scan(char* buf, int buf_len, int is_blocking) override;
    int scan(int* buf, int is_blocking) override;
};

#endif