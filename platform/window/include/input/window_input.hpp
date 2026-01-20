#ifndef __WINDOW_INPUT_HPP__
#define __WINDOW_INPUT_HPP__
#include <i_input_handler.hpp>

class WindowInput : public IInputHandler
{
  public:
    // 버퍼에 값이 있으면 char 로 넘겨주고 없으면 -1 반환
    int scan();

    ~WindowInput();
};
#endif