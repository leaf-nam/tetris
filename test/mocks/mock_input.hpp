#include "i_input_handler.hpp"

class MockInput : public IInputHandler
{
public:
    int key = 0;

    void input(int user_input)
    {
        key = user_input;
    }

    int scan() override 
    {
        return key;
    }
};