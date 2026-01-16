#ifndef __TERMINAL_INPUT_HPP__
#define __TERMINAL_INPUT_HPP__
#include <i_input_handler.hpp>

class TerminalInput : public IInputHandler
{
    public:
        //���ۿ� ���� ������ char �� �Ѱ��ְ� ������ -1 ��ȯ
        int scan();

        ~TerminalInput();
};
#endif