#ifndef __KEY_MAPPER_HPP__
#define __KEY_MAPPER_HPP__

#include <unordered_map>
#include "util/action.hpp"

class KeyMapper
{
private:
    std::unordered_map<char, Action> key_map;
    KeyMapper(); // 추후 사용자 설정 받아서 변경도 가능하게 수정
public:
    static KeyMapper& get_instance();
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;
    Action map_key(char key);
};

#endif