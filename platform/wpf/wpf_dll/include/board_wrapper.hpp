#ifndef __BOARD_WRAPPER_HPP__
#define __BOARD_WRAPPER_HPP__
extern "C"
{
    struct BoardWrapper
    {
    public:
        const int8_t* cells;
        int xs;
        int ys;
    };
}
#endif
