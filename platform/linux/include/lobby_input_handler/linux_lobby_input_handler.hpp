#ifndef __LINUX_LOBBY_INPUT_HANDLER_HPP__
#define __LINUX_LOBBY_INPUT_HANDLER_HPP__

#include "i_lobby_input_handler.hpp"
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

class LinuxLobbyInputHandler : public ILobbyInputHandler
{
  private:
    termios old;

    void enable_noncanonical_noecho();
    void restore();
    int _kbhit();
  public:
    int scan(char* buf, int buf_len, int is_blocking) override;
    int scan(int* buf, int is_blocking) override;
};

#endif
