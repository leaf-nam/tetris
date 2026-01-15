#ifndef __LINUX_INPUT_HANDLER_HPP__
#define __LINUX_INPUT_HANDLER_HPP__

#include "api/i_input_handler.hpp"

class LinuxInput : public IInputHandler
{
    public:
    
    LinuxInput();

    /**
     * @brief 버퍼로부터 문자열 입력 받기
     * @return 입력 없을 때 "\0", 입력 있을 때 해당 문자열 반환
     */
    char scan() override;

    ~LinuxInput();
};

#endif
