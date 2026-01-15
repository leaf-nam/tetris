#ifndef __TERMINAL_INPUT_HPP__
#define __TERMINAL_INPUT_HPP__
#include "api/i_input_handler.hpp"

class TerminalInput : public IInputHandler
{
    public:
        //버퍼에 값이 있으면 char 로 넘겨주고 없으면 -1 반환
        char scan();

        ~TerminalInput();
};
#endif