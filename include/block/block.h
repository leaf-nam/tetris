#include <cstdint>
#include <iostream>

#define BLOCK_MAX_ROW 4
#define BLOCK_MAX_COL 4
#define TYPE_CNT 7

struct BlockData {
    uint8_t type;
    uint8_t turn;
    uint8_t r;
    uint8_t c;
    uint16_t row;

    BlockData(unsigned int);
    void turn_right();
    void turn_left();
    void print();

private:
    void render(uint8_t type);
};