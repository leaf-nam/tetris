#ifndef __I_SETTING_INPUT_HANDLER_HPP__
#define __I_SETTING_INPUT_HANDLER_HPP__

#include <string>

class ISettingInputHandler
{
  public:
    /**
     * @brief 버퍼로부터 문자열 입력 받기
     * @return 입력 없을 때 "\0", 입력 있을 때 해당 문자열 반환
     */
    virtual int scan() = 0;

    virtual std::string get_line() = 0;

    /**
     * @brief 소멸자
     */
    virtual ~ISettingInputHandler() = default;
};
#endif
