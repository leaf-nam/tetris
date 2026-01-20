#include "input/linux_input.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

static struct termios initial_settings, new_settings;

LinuxInput::LinuxInput()
{
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;

    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;

    tcsetattr(0, TCSANOW, &new_settings);
}

int LinuxInput::scan()
{
    unsigned char ch;
    int nread = read(0, &ch, 1);

    if (ch == 27) {
        unsigned char seq[2];
        read(0, &seq[0], 1);
        read(0, &seq[1], 1);

        if (seq[0] == 91) {
            if (seq[1] == 'A') return Arrow::KEY_UP;
            if (seq[1] == 'B') return Arrow::KEY_DOWN;
            if (seq[1] == 'C') return Arrow::KEY_RIGHT;
            if (seq[1] == 'D') return Arrow::KEY_LEFT;
        }
    }

    return (nread == 1) ? ch : 0;
}

LinuxInput::~LinuxInput() { tcsetattr(0, TCSANOW, &initial_settings); }
