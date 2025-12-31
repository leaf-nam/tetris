#include <cstdint>
#include <iostream>

#define MAX_ROW 20
#define MAX_COL 10

struct Board2D {
    uint16_t row[MAX_ROW];
};

class Board {
private:
    Board2D board;
    int max_row;

public:
    Board();
    bool exist(int, int);
    void set(int, int);
    void print();
};