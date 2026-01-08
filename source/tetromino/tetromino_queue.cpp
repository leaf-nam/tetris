#include "tetromino/tetromino_queue.hpp"
#include "tetromino/tetromino.hpp"
#include "util/rand_gen.hpp"
#include <iostream>

using namespace std;

TetrominoQueue::TetrominoQueue()
{
    RandGen& rand_gen = RandGen::get_instance();
    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);
    
    for(int i = 0; i < size_of_tetrominos; ++i)
    {
        tetrominos[i] = rand_gen.get_rand_int() % 7;
    }
}

TetrominoQueue& TetrominoQueue::get_instance()
{
    static TetrominoQueue instance;
    return instance;
}

int TetrominoQueue::get_new_tetromino()
{
    RandGen& rand_gen = RandGen::get_instance();
    int new_tetromino = tetrominos[0];
    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);

    for(int i = 1; i < size_of_tetrominos; ++i)
    {
        tetrominos[i - 1] = tetrominos[i];
    }

    tetrominos[size_of_tetrominos - 1] = rand_gen.get_rand_int() % 7;

    return new_tetromino;
}

void TetrominoQueue::set_new_tetromino(int new_tetromino)
{
    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);

    for(int i = 0; i < size_of_tetrominos - 1; ++i)
    {
        tetrominos[i + 1] = tetrominos[i];
    }

    tetrominos[0] = new_tetromino;
}

const int* TetrominoQueue::get_tetrominos()
{
    return tetrominos;
}

void TetrominoQueue::draw_tetromino_queue()
{
    static bool called = false;
    mino mino_mask = 0b1111000000000000;
    mino mino_shape;
    static const uint16_t left_edge = 1u << 12;
    static const uint16_t right_edge = 1u << 3;
    uint16_t mino_row;

    std::cout << "\x1b[15A";
    std::cout << "\x1b[11C";
    std::cout << "NEXT\n";
    for (int tetromino_num = 0; tetromino_num < 3; ++tetromino_num)
    {
        mino_shape = TETROMINO[tetrominos[tetromino_num]][0];
        for (int r = 0; r < 4; ++r)
        {
            std::cout << "\x1b[11C";
            for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
            {
                cout << " ";
            }

            mino_row = (mino_mask >> r * 4) & mino_shape;
            mino_row >>= (3 - r) * 4;
            mino_row <<= (9);

            std::cout << "\x1b[10D";
            for (uint16_t mask = left_edge; mask >= right_edge; mask >>= 1)
            {
                cout << ((mino_row & mask) ? "■" : "\x1b[C");
            }
            cout << endl;
        }
    }
    std::cout << "\x1b[16B";
    std::cout << "\x1b[11D";
    cout << flush;
}
