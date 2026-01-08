#ifndef __TERMINAL_INPUT_HPP__
#define __TERMINAL_INPUT_HPP__
#include "api/IInputHandler.hpp"
//#include <string>
class TInput : IInputHandler
{
    public:
        string scan();
        ~TInput();
};