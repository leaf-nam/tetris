#ifndef __I_LOBBY_INPUT_HANDLER_HPP__
#define __I_LOBBY_INPUT_HANDLER_HPP__

class ILobbyInputHandler
{
  public:
    // is_blocking == 1 => true, 0 => false
    virtual int scan(char* buf, int buf_len, int is_blocking) = 0;
    virtual int scan(int* buf, int is_blocking) = 0;
    ~ILobbyInputHandler() = default;
};

#endif