#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include <cstdint>
#include <cstddef>
#include <utility>

typedef std::uint16_t mino;

extern mino *TETROMINO[7];

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
    const mino get_shape() const;
    const mino get_shape(int rot) const;
    const int get_rotation() const;
    void set_mino_type(int type);
    const int get_mino_type() const;
};

#endif
