#include "tetromino/tetromino_queue.hpp"
#include "util/rand_gen.hpp"

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

    tetrominos[size_of_tetrominos] = rand_gen.get_rand_int() % 7;

    return new_tetromino;
}

const int* TetrominoQueue::get_tetrominos()
{
    return tetrominos;
}
