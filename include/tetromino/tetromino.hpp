#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include <cstdint>
#include <cstddef>
#include <utility>

inline constexpr int MINO_SIZE = 4;
inline constexpr int MINO_ROT = 4;
inline constexpr int MINO_KIND = 7;

typedef int mino_row[MINO_SIZE];
typedef mino_row mino[MINO_SIZE];
typedef mino mino_shapes[MINO_ROT];

extern mino* TETROMINO[MINO_KIND];

enum MinoType 
{
    I = 0,
    O,
    Z,
    S,
    J,
    L,
    T,
    OBSTACLE, // 테트로미노에 속하지 않는 블록 (장애물, 쓰레기 등)
    NONE,
};

class Tetromino
{
    private:
    int mino_type;
    int rotation;
    std::pair<int, int> pos;

    public:
    Tetromino();
    void init_mino(int type);
    void set_rotation(int rot);
    void set_pos(int new_r, int new_c);
    const std::pair<int, int> get_pos() const;
    const mino& get_shape() const;
    const mino& get_shape(int rot) const;
    const int get_rotation() const;
    void set_mino_type(int type);
    const int get_mino_type() const;
};

#endif
