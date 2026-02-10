#ifndef __I_MENU_INPUT_HANDLER_HPP__
#define __I_MENU_INPUT_HANDLER_HPP__

class IMenuInputHandler
{
  public:
    /**
     * @brief 버퍼로부터 문자열 입력 받기
     * @return 입력 없을 때 "\0", 입력 있을 때 해당 문자열 반환
     */
    virtual int scan() = 0;

    /**
     * @brief 소멸자
     */
    virtual ~IMenuInputHandler() = default;
};
#endif
