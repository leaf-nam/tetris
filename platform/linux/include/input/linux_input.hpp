#ifndef __LINUX_INPUT_HPP__
#define __LINUX_INPUT_HPP__

#include <i_input_handler.hpp>
#include <i_menu_input_handler.hpp>
#include <i_setting_input_handler.hpp>
#include <string>
#include <termios.h>

class LinuxInput : public IInputHandler, public IMenuInputHandler, public ISettingInputHandler
{
  private:
    bool inited;
    termios old;

    void enable_noncanonical_noecho();
    void restore();
    int _kbhit();
  public:
    /**
     * @brief 버퍼로부터 문자열 입력 받기
     * @return 입력 없을 때 "\0", 입력 있을 때 해당 문자열 반환
     */
    int scan() override;

    std::string get_line() override;
};

#endif
