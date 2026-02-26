#include "lobby_input_handler/linux_lobby_input_handler.hpp"
#include <stdlib.h>
#include <string.h>
#include <climits>
#include <stdio.h>
#include <iostream>


void LinuxLobbyInputHandler::enable_noncanonical_noecho() {
    termios t;
    
    tcgetattr(STDIN_FILENO, &old);
    t = old;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN]  = 0;
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

int LinuxLobbyInputHandler::_kbhit() {
    timeval tv{0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int r = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
    return (r > 0) && FD_ISSET(STDIN_FILENO, &fds);
}

int LinuxLobbyInputHandler::scan(char* buf, int buf_len, int is_blocking)
{
    char line[1024];
    int copied = 0;

    if (buf_len <= 0) return 0;

    if (is_blocking != 1)
    {
        enable_noncanonical_noecho();
        if (_kbhit() == 0)
	{
	    restore();
            return 0;
	}
    }
    restore();

    if (!std::fgets(line, sizeof(line), stdin))
        return 0;

    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r')
            break;
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
        if (_kbhit() == 0)
	{
	    restore();
            return 0;
	}
    }
    restore();

    if (!std::fgets(line, sizeof(line), stdin))
        return 0;

    errno = 0;
    long v = std::strtol(line, NULL, 10);
    bool ok = (errno == 0) && (v >= INT_MIN) && (v <= INT_MAX);

    if (ok) *buf = (int)v;

    return ok ? 1 : 0;
}
