#ifndef __TERMINAL_INPUT_HPP__
#define __TERMINAL_INPUT_HPP__
#include "api/IInputHandler.hpp"

class TInput : public IInputHandler
{
    public:
        string scan();

        ~TInput();
};