#include "game_rule/key_mapper.hpp"

KeyMapper::KeyMapper()
{
    key_map[Arrow::KEY_LEFT] = Action::LEFT;
    key_map['a'] = Action::LEFT;

    key_map[Arrow::KEY_RIGHT] = Action::RIGHT;
    key_map['d'] = Action::RIGHT;

    key_map[Arrow::KEY_DOWN] = Action::DROP;
    key_map['s'] = Action::DROP;

    key_map['z'] = Action::ROTATE_CCW;
    key_map['q'] = Action::ROTATE_CCW;

    key_map['x'] = Action::ROTATE_CW;
    key_map[Arrow::KEY_UP] = Action::ROTATE_CW;
    key_map['e'] = Action::ROTATE_CW;

    key_map[' '] = Action::HARD_DROP;
    key_map['f'] = Action::HARD_DROP;

    key_map['w'] = Action::SWAP;
    key_map['c'] = Action::SWAP;
}

Action KeyMapper::map_key(int key)
{
    auto it = key_map.find(key);
    return (it != key_map.end()) ? it->second : Action::INVALID;
}