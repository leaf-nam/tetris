#ifndef __ACTION_RESOLVER_HPP__
#define __ACTION_RESOLVER_HPP__

#include <vector>
#include "util/action.hpp"

typedef std::tuple<int, int, int> Pose;

inline constexpr int KICK_TEST = 5;

extern const std::pair<int, int> KICK_TABLE_I[4][2][KICK_TEST];
extern const std::pair<int, int> KICK_TABLE_JLSTZ[4][2][KICK_TEST];

class ActionResolver
{
private:
    const std::pair<int, int>* get_kick_table(int mino_type, int curr_rot, int rot_dir) const;
    int rotate(int curr_rot, int a);
public:
    Pose resolve_move(int curr_r, int curr_c, int curr_rot, int a);
    std::vector<Pose> resolve_rotation(int curr_r, int curr_c, int curr_rot, int mino_type, int a, bool enable_kick);
};

#endif