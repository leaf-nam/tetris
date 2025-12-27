#include "tetromino.hpp"

Tetromino::Tetromino(unsigned int block_type) : shape(TETROMINO[block_type]), rotation(0), x(0), y(0) {}

void Tetromino::rotate(int rot) 
{
    rotation = rot;
}

void Tetromino::move(int dir) 
{

}