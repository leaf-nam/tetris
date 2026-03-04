#include "input/linux_input.hpp"

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>


void LinuxInput::enable_noncanonical_noecho() {
    termios t;
    
    if (tcgetattr(STDIN_FILENO, &old) == -1) return;
    t = old;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN]  = 0;
    t.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == -1) return;
    inited = true;
}

void LinuxInput::restore()
{
    if (inited) tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

int LinuxInput::_kbhit() {
    timeval tv{0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int r = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
    return (r > 0) && FD_ISSET(STDIN_FILENO, &fds);
}

int LinuxInput::scan()
{
    unsigned char ch = '\0';

    enable_noncanonical_noecho();
    while(_kbhit() != 0) {
        read(0, &ch, 1);
        if (ch == 27) {
            unsigned char seq[2];
            read(0, &seq[0], 1);
            read(0, &seq[1], 1);

            if (seq[0] == '[') {
                if (seq[1] == 'A') return Arrow::KEY_UP;
                if (seq[1] == 'B') return Arrow::KEY_DOWN;
                if (seq[1] == 'C') return Arrow::KEY_RIGHT;
                if (seq[1] == 'D') return Arrow::KEY_LEFT;
            }
        }
    }
    restore();

    return (int)ch;
}

std::string LinuxInput::get_line()
{
    std::string s;
    getline(std::cin, s);

    return s;
}
