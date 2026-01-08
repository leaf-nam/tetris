#include "input/linux_input.hpp"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <iostream>

static struct termios initial_settings, new_settings;

LinuxInput::LinuxInput()
{   
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

char LinuxInput::scan()
{
    unsigned char ch;
    int nread;
 
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1) return ch;
    return 0;
}

LinuxInput::~LinuxInput()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
