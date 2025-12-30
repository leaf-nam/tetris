#include "board/board.h"
#include <bitset>

using namespace std;

Board::Board() {
    board = {0};
}

void Board::set(int r, int c) {
    board.row[r] |= (1 << c);
}

bool Board::exist(int r, int c) {
    return (board.row[r] & (1 << c)) > 0;
}

void Board::print() {
    for (int i = MAX_ROW - 1; i >= 0; --i) {
        std::bitset<MAX_COL> bits(board.row[i]);
        cout << bits << '\n';
    }
}