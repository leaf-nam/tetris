#pragma once

#include <string>

using namespace std;

class IInputHandler {
public:

    /**
     * @brief 버퍼로부터 문자열 입력 받기
     * @return 입력 없을 때 "\0", 입력 있을 때 해당 문자열 반환
     */
    virtual string scan();

    /**
     * @brief 소멸자
     */
    virtual ~IInputHandler();

};