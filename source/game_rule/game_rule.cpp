#include "game_rule/game_rule.hpp"

using namespace std;

RuleEngine::RuleEngine(Board& board) : level_game_time(0), current_level(1), board(board), enable_kick(true)
{
    // 120 == 1 minute, when timer is 500ms
    time_for_level_up[0] = 0;
    time_for_level_up[1] = 0;
    time_for_level_up[2] = 120;
    time_for_level_up[3] = 120;
    time_for_level_up[4] = 120;
    time_for_level_up[5] = 120;
    time_for_level_up[6] = 120;
    time_for_level_up[7] = 120;
    time_for_level_up[8] = 120;
    time_for_level_up[9] = 120;
    time_for_level_up[10] = 120;
}

/**
 * @brief 테트로미노의 원래 회전과 회전 방향에 따른 wall kick 테이블을 반환함
 * @param mino_type
 * @param curr_rot
 * @param rot_dir 반드시 0 (CW), 1(CCW) 중에 하나
 * @note
 * - O형 테트로미노는 kick table이 없으므로 입력하면 안됨
 * - 좌표계는 (row, col)이며, row 증가하면 downward, col 증가하면 rightward임
 */
const std::pair<int, int>* RuleEngine::get_kick_table(int mino_type, int curr_rot, int rot_dir)
{
    switch (mino_type)
    {
        case MinoType::I: return KICK_TABLE_I[curr_rot][rot_dir];
        case MinoType::J:
        case MinoType::L:
        case MinoType::S:
        case MinoType::T:
        case MinoType::Z: return KICK_TABLE_JLSTZ[curr_rot][rot_dir];
        default: return nullptr;
    }
}


/**
 * @brief 유저 인풋을 처리하여 board에 지시
 * @param user_input
 */
void RuleEngine::process(int user_input)
{
    auto [curr_r, curr_c] = board.get_active_mino_pos();
    int curr_rot = board.get_active_mino_rotation();
    int new_r, new_c, new_rot;
    int mino_type, rot_dir;
    const pair<int, int>* kick_table;

    if (user_input == Action::DROP)
    {
        new_r = curr_r + 1;
        new_c = curr_c;
        new_rot = curr_rot;

        if (board.can_place_mino(new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
        else board.update_board();
    }
    else if (user_input == Action::LEFT || user_input == Action::RIGHT)
    {
        new_r = curr_r;
        new_c = (user_input == Action::LEFT ? curr_c - 1 : curr_c + 1);
        new_rot = curr_rot;
        
        if (board.can_place_mino(new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
    }
    else if (user_input == Action::ROTATE_CCW || user_input == Action::ROTATE_CW)
    {
        mino_type = board.get_active_mino_type();
        new_rot = (user_input == Action::ROTATE_CW ? curr_rot + 1 : curr_rot - 1);

        if (new_rot == -1) new_rot = 3;
        else if (new_rot == 4) new_rot = 0;

        if (enable_kick && mino_type != MinoType::O)
        {
            rot_dir = (user_input == Action::ROTATE_CW ? 0 : 1);
            kick_table = get_kick_table(mino_type, curr_rot, rot_dir);
            if (kick_table == nullptr) return;

            for (int i = 0; i < KICK_TEST; ++i)
            {
                new_r = curr_r + kick_table[i].first;
                new_c = curr_c + kick_table[i].second;
                if (board.can_place_mino(new_r, new_c, new_rot))
                {
                    board.set_active_mino_pos(new_r, new_c);
                    board.set_active_mino_rotation(new_rot);
                    break;
                }
            }
        }
        else
        {
            new_r = curr_r;
            new_c = curr_c;

            if (board.can_place_mino(new_r, new_c, new_rot))
            {
                board.set_active_mino_pos(new_r, new_c);
                board.set_active_mino_rotation(new_rot);
            }
        }
    }
    else if (user_input == Action::HARD_DROP)
    {
        new_r = curr_r;
        new_c = curr_c;
        new_rot = curr_rot;
        while (board.has_active_mino())
        {
            if (board.can_place_mino(++new_r, new_c, new_rot)) board.set_active_mino_pos(new_r, new_c);
            else board.update_board();
        }
    }
    else if (user_input == Action::SWAP)
    {
        if (board.has_swaped_mino() == false)
            board.swap_mino();
        else return;
    }
}

int RuleEngine::update_score(Board& board)
{
    int score = 0;
    int base_score = 100;
    int r = 21;
    while (r >= 2) 
    {
        if (board.is_line_full(r))
        {
            board.delete_line(r);
            score += base_score;
        }
        else
        {
            --r;
        }
    }
    return score;
}

bool RuleEngine::is_game_over(const uint16_t *board)
{
    for (int r = 0; r < 2; ++r) 
    {
        if (board[r]) return true;
    }

    return false;
}

bool RuleEngine::time_and_level_update()
{
    level_game_time++;
    if ((current_level < 10) && (time_for_level_up[current_level + 1] <= level_game_time))
    {
        level_game_time = 0;
        current_level++;
        return true;
    }
    return false;
}

int RuleEngine::get_level()
{
    return current_level;
}
