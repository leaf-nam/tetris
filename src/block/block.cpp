#include "block/block.h"
#include <bitset>

using namespace std;

const uint16_t BlockData::shapes[7][4] = {
    {0xCC00, 0xCC00, 0xCC00, 0xCC00}, // O
    {0x8888, 0xF000, 0x8888, 0xF000}, // I
    {0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
    {0x4460, 0xE800, 0xC440, 0x2E00}, // L
    {0xC880, 0xE200, 0x44C0, 0x8E00}, // J
    {0x6C00, 0x8C40, 0x6C00, 0x8C40}, // S
    {0xC600, 0x4C80, 0xC600, 0x4C80}  // Z
};\

BlockData::BlockData(unsigned int type):turn(0), r(0), c(0) 
{
    this->type = type;
    render_init(type);
}

void BlockData::render_init(uint8_t type) 
{
    row = shapes[type][0];
}

void BlockData::render_turn() 
{
    row = shapes[type][turn];
}


void BlockData::turn_right() 
{
    turn = (turn + 1) & 3;
    render_turn();
}

void BlockData::turn_left() 
{
    turn = (turn + 3) & 3;
    render_turn();
}

void BlockData::print() 
{
    for (int r = 0; r < BLOCK_MAX_ROW; ++r) 
    {
        for (int c = 0; c < BLOCK_MAX_COL; ++c) 
        {
            int bit = 15 - (r * BLOCK_MAX_ROW + c);
            cout << ((row >> bit) & 1);
        }
        cout << '\n';
    }
}