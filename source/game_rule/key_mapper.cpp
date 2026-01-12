#include "game_rule/key_mapper.hpp"

KeyMapper::KeyMapper()
{
    key_map['a'] = Action::LEFT;
    key_map['s'] = Action::RIGHT;
    key_map['d'] = Action::DROP;
    key_map['q'] = Action::ROTATE_CCW;
    key_map['e'] = Action::ROTATE_CW;
    key_map['f'] = Action::HARD_DROP;
    key_map['w'] = Action::SWAP; 
}

Action KeyMapper::map_key(char key)
{
    auto it = key_map.find(key);
    return (it != key_map.end()) ? it->second : Action::INVALID;
}