#include<iostream>
#include "board/board.hpp"
#include "tetromino/tetromino.hpp"
#include <vector>
class TRenderer {
public:
    virtual ~TRenderer() {}
    virtual bool initialize() = 0; 
    virtual void clear() = 0;
    virtual void draw(const Board& board, const std::vector<Tetromino>& queue) = 0;
    virtual void display() = 0;    
};