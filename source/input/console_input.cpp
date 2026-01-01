#include "input/console_input.hpp"
#include "input/action.hpp"

static struct termios initial_settings, new_settings;
 
Input::Input()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
 
Input::~Input()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
 
int Input::_kbhit()
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

int Input::console_input()
{
    int key = _kbhit();
    if(key)
    {
        switch(key)
        {
            case 'a': return Action::LEFT;
            case 's': return Action::DROP;
            case 'd': return Action::RIGHT;
            case 'q': return Action::ROTATE_CCW;
            case 'e': return Action::ROTATE_CW;
            case 'f': return Action::HARD_DROP;
        }
    }

    return -1;
}
