#include "board/board.hpp"
#include "render/pos.hpp"
#include "tetromino/tetromino.hpp"

#include <vector>

class ShadowMaker
{
  public:
    std::vector<Pos> get_shadow_pos(const Board& board, const Tetromino& tetromino);
    bool is_shadow(std::vector<Pos>, Pos);

  private:
    std::vector<Pos> get_mino_pos(const Tetromino& tetromino);
    bool is_collide(const BoardT& board, const Tetromino& tetromino, Pos);
};
