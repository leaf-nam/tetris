#include "input/linux_lobby_input_handler.hpp"

#include <climits>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LinuxLobbyInputHandler::enable_noncanonical_noecho()
{
    termios t;

    tcgetattr(STDIN_FILENO, &old);
    t = old;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 0;
    t.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void LinuxLobbyInputHandler::restore()
{
    termios t;

    tcgetattr(STDIN_FILENO, &old);
    t = old;
    t.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int LinuxLobbyInputHandler::_kbhit()
{
    timeval tv{0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int r = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
    return (r > 0) && FD_ISSET(STDIN_FILENO, &fds);
}

int LinuxLobbyInputHandler::scan()
{
    enable_noncanonical_noecho();

    unsigned char ch = '\0';

    while (_kbhit() != 0) {
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

    return (int) ch;
}

int LinuxLobbyInputHandler::scan(char* buf, int buf_len, int is_blocking)
{
    char line[1024];
    int copied = 0;

    if (buf_len <= 0) return 0;

    if (is_blocking != 1) {
        enable_noncanonical_noecho();
        if (_kbhit() == 0) {
            restore();
            return 0;
        }
    }
    restore();

    if (!std::fgets(line, sizeof(line), stdin)) return 0;

    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r') break;
        if (copied < buf_len - 1) buf[copied++] = line[i];
    }
    buf[copied] = '\0';

    return (copied > 0) ? 1 : 0;
}

int LinuxLobbyInputHandler::scan(int* buf, int is_blocking)
{
    char line[1024];

    if (!buf) return 0;

    if (is_blocking != 1) {
        enable_noncanonical_noecho();
        if (_kbhit() == 0) {
            restore();
            return 0;
        }
    }
    restore();

    if (!std::fgets(line, sizeof(line), stdin)) return 0;

    errno = 0;
    long v = std::strtol(line, NULL, 10);
    bool ok = (errno == 0) && (v >= INT_MIN) && (v <= INT_MAX);

    if (ok) *buf = (int) v;

    return ok ? 1 : 0;
}
