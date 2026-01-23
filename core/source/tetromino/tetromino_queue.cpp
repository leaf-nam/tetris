#include "tetromino/tetromino_queue.hpp"

using namespace std;

TetrominoQueue::TetrominoQueue()
{
    init();
}

void TetrominoQueue::init()
{
    draw_count = 0;

    int size_of_tetrominos = sizeof(curr_tetrominos) / sizeof(curr_tetrominos[0]);
    for (int i = 0; i < size_of_tetrominos; ++i) {
        curr_tetrominos[i] = i;
        next_tetrominos[i] = i;
    }

    shuffle(curr_tetrominos, curr_tetrominos + size_of_tetrominos, rng);
    shuffle(next_tetrominos, next_tetrominos + size_of_tetrominos, rng);
}

TetrominoQueue& TetrominoQueue::get_instance()
{
    static TetrominoQueue instance;
    return instance;
}

int TetrominoQueue::get_new_tetromino()
{
    int size_of_tetrominos = sizeof(curr_tetrominos) / sizeof(curr_tetrominos[0]);
    int new_tetromino;

    if (draw_count >= size_of_tetrominos) {
        shuffle(next_tetrominos, next_tetrominos + size_of_tetrominos, rng);
        draw_count = 0;
    }

    new_tetromino = curr_tetrominos[0];

    for (int i = 1; i < size_of_tetrominos; ++i) {
        curr_tetrominos[i - 1] = curr_tetrominos[i];
    }

    curr_tetrominos[size_of_tetrominos - 1] = next_tetrominos[draw_count];
    draw_count++;

    return new_tetromino;
}

void TetrominoQueue::set_new_tetromino(int new_tetromino)
{
    int size_of_tetrominos = sizeof(curr_tetrominos) / sizeof(curr_tetrominos[0]);

    for (int i = 0; i < size_of_tetrominos; ++i) {
        if (new_tetromino == curr_tetrominos[i]) {
            int temp = curr_tetrominos[0];
            curr_tetrominos[0] = new_tetromino;
            curr_tetrominos[i] = temp;
        }
    }
}

const int* TetrominoQueue::get_tetrominos() { return curr_tetrominos; }
