#ifndef __CONSOLE_INPUT_HPP__
#define __CONSOLE_INPUT_HPP__

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

class Input
{
    private:
    int _kbhit();

    public:
    Input();
    ~Input();
    int console_input();
};

#endif