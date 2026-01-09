#ifndef __KEY_MAPPER_HPP__
#define __KEY_MAPPER_HPP__

#include <unordered_map>
#include "input/action.hpp"

class KeyMapper
{
private:
    std::unordered_map<char, Action> key_map;
public:
    KeyMapper(); // 추후 사용자 설정 받아서 변경도 가능하게 수정
    Action map_key(char key);
};

#endif