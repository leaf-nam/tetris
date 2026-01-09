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
