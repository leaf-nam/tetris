#include "util/shadow_maker.hpp"

#include "render/pos.hpp"

#include <vector>

using namespace std;

bool ShadowMaker::isShadow(vector<Pos> shadows, Pos pos)
{
    for (auto shadow : shadows) {
        if (shadow.y == pos.y && shadow.x == pos.x) {
            return true;
        }
    }
    return false;
}

vector<Pos> ShadowMaker::get_shadow_pos(const Board& board, const Tetromino& tetromino)
{
    vector<Pos> shadow_pos;
    int r = tetromino.get_pos().first;
    int c = tetromino.get_pos().second;

    const BoardT& board2D = board.get_board();
    auto mino = tetromino.get_mino_type();

    // 충돌지점 찾기
    while (!is_collide(board2D, tetromino, {c, ++r}))
        ;

    // 그림자 위치 반환
    int delta = r - tetromino.get_pos().first - 1;
    vector<Pos> base_position = get_mino_pos(tetromino);
    for (auto& p : base_position) {
        shadow_pos.push_back({p.x, p.y + delta});
    }

    return shadow_pos;
}

vector<Pos> ShadowMaker::get_mino_pos(const Tetromino& tetromino)
{
    vector<Pos> ret;

    for (int i = 0; i < MINO_SIZE; ++i) {
        for (int j = 0; j < MINO_SIZE; ++j) {
            if (tetromino.get_shape()[i][j] != 0) {
                ret.push_back({tetromino.get_pos().second + j, tetromino.get_pos().first + i});
            }
        }
    }

    return ret;
}

bool ShadowMaker::is_collide(const BoardT& board, const Tetromino& tetromino, Pos pos)
{
    for (int i = 0; i < MINO_SIZE; ++i) {
        for (int j = 0; j < MINO_SIZE; ++j) {
            if (tetromino.get_shape()[i][j] != 0) {
                int board_r = pos.y + i;
                int board_c = pos.x + j;

                // 보드 경계 검사
                if (board_r < 0 || board_r >= BOARD_ROW || board_c < 0 || board_c >= BOARD_COL) {
                    return true;
                }

                // 보드에 이미 블록이 있는지 검사
                if (board[board_r][board_c] != 8) {
                    return true;
                }
            }
        }
    }

    return false;
}
