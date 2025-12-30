#include "block/block.h"
#include <bitset>

using namespace std;

BlockData::BlockData(unsigned int type):turn(0), r(0), c(0) {
    this->type = type;
    render(type);
}

void BlockData::render(uint8_t type) {
    switch(type) {
        case 0: row = 0xCC00; break;
        case 1: row = 0x8888; break;
        case 2: row = 0x4E00; break;
        case 3: row = 0xC880; break;
        case 4: row = 0xC440; break;
        case 5: row = 0x4C80; break;
        case 6: row = 0x8C40; break;
    }
}

void BlockData::turn_right() {
    turn = (turn + 1) & 3;
}

void BlockData::turn_left() {
    turn = (turn + 3) & 3;
}

void BlockData::print() {
    for (int r = 0; r < BLOCK_MAX_ROW; ++r) {
        for (int c = 0; c < BLOCK_MAX_COL; ++c) {
            int bit = 15 - (r * BLOCK_MAX_ROW + c);
            cout << ((row >> bit) & 1);
        }
        cout << '\n';
    }
}