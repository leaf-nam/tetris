#include "tetromino/tetromino_queue.hpp"
#include "tetromino/tetromino.hpp"
#include "util/rand_gen.hpp"
#include <iostream>

using namespace std;

TetrominoQueue::TetrominoQueue()
{
    draw_count = 0;

    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);
    for (int i = 0; i < size_of_tetrominos; ++i)
    {
        tetrominos[i] = i;
    }
    
    mt19937 rng(random_device{}());
    shuffle(tetrominos, tetrominos + size_of_tetrominos, rng);
}

TetrominoQueue& TetrominoQueue::get_instance()
{
    static TetrominoQueue instance;
    return instance;
}

int TetrominoQueue::get_new_tetromino()
{
    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);
    int new_tetromino;

    if (draw_count >= size_of_tetrominos)
    {
        mt19937 rng(random_device{}());
        shuffle(tetrominos, tetrominos + size_of_tetrominos, rng);
        draw_count = 0;
    }

    new_tetromino = tetrominos[0];

    for (int i = 1; i < size_of_tetrominos; ++i)
    {
        tetrominos[i - 1] = tetrominos[i];
    }

    tetrominos[size_of_tetrominos - 1] = new_tetromino;
    draw_count++;

    return new_tetromino;
}

void TetrominoQueue::set_new_tetromino(int new_tetromino)
{
    int size_of_tetrominos = sizeof(tetrominos) / sizeof(tetrominos[0]);

    for(int i = 0; i < size_of_tetrominos; ++i)
    {
        if (new_tetromino == tetrominos[i])
        {
            int temp = tetrominos[0];
            tetrominos[0] = new_tetromino;
            tetrominos[i] = temp;
        }
    }
}

const int* TetrominoQueue::get_tetrominos()
{
    return tetrominos;
}
