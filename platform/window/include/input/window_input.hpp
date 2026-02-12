#ifndef __WINDOW_INPUT_HPP__
#define __WINDOW_INPUT_HPP__

#include <i_input_handler.hpp>
#include <i_menu_input_handler.hpp>
#include <i_setting_input_handler.hpp>
#include <string>

class WindowInput : public IInputHandler, public IMenuInputHandler, public ISettingInputHandler
{
  public:
    // 버퍼에 값이 있으면 char 로 넘겨주고 없으면 -1 반환
    int scan() override;

    std::string get_line() override;

    ~WindowInput();
};
#endif //!__WINDOW_INPUT_HPP__
