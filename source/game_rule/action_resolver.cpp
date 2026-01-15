#include "game_rule/action_resolver.hpp"

#include "assert.h"
#include "tetromino/tetromino.hpp"

using namespace std;

/**
 * @brief 테트로미노의 원래 회전과 회전 방향에 따른 wall kick 테이블을 반환함
 * @param mino_type
 * @param curr_rot
 * @param rot_dir 반드시 0 (CW), 1(CCW) 중에 하나
 * @note
 * - O형 테트로미노는 kick table이 없으므로 입력하면 안됨
 * - 좌표계는 (row, col)이며, row 증가하면 downward, col 증가하면 rightward임
 */
const std::pair<int, int>* ActionResolver::get_kick_table(int mino_type, int curr_rot,
                                                          int rot_dir) const
{
    assert(curr_rot >= 0 && curr_rot < 4);
    assert(rot_dir >= 0 && rot_dir < 2);

    switch (mino_type) {
    case MinoType::I:
        return KICK_TABLE_I[curr_rot][rot_dir];
    case MinoType::J:
    case MinoType::L:
    case MinoType::S:
    case MinoType::T:
    case MinoType::Z:
        return KICK_TABLE_JLSTZ[curr_rot][rot_dir];
    default:
        return nullptr;
    }
}

int ActionResolver::rotate(int curr_rot, int input)
{
    int new_rot = (input == Action::ROTATE_CW ? curr_rot + 1 : curr_rot - 1);

    if (new_rot == -1)
        new_rot = 3;
    else if (new_rot == 4)
        new_rot = 0;

    return new_rot;
}

Pose ActionResolver::resolve_move(int curr_r, int curr_c, int curr_rot, int input)
{
    switch (input) {
    case Action::DROP:
        return {curr_r + 1, curr_c, curr_rot};
    case Action::LEFT:
        return {curr_r, curr_c - 1, curr_rot};
    case Action::RIGHT:
        return {curr_r, curr_c + 1, curr_rot};
    default:
        return {curr_r, curr_c, curr_rot};
    }
}

vector<Pose> ActionResolver::resolve_rotation(int curr_r, int curr_c, int curr_rot, int mino_type,
                                              int input, bool enable_kick)
{
    int rot_dir = (input == Action::ROTATE_CW ? 0 : 1);
    int new_rot = rotate(curr_rot, input);

    const pair<int, int>* kick_table;
    vector<Pose> poses;

    poses.reserve(KICK_TEST);

    switch (mino_type) {
    case MinoType::I:
    case MinoType::J:
    case MinoType::L:
    case MinoType::S:
    case MinoType::T:
    case MinoType::Z: {
        if (enable_kick) {
            kick_table = get_kick_table(mino_type, curr_rot, rot_dir);
            for (int i = 0; i < KICK_TEST; ++i) {
                auto [dr, dc] = kick_table[i];
                poses.emplace_back(curr_r + dr, curr_c + dc, new_rot);
            }
        }
        break;
    }
    default: {
        poses.emplace_back(curr_r, curr_c, curr_rot);
        break;
    }
    }

    return poses;
}