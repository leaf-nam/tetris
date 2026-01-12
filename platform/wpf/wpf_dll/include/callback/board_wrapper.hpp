#ifndef __BOARD_WRAPPER_HPP__
#define __BOARD_WRAPPER_HPP__
extern "C"
{
    struct BoardWrapper
    {
    public:
        int* board;
        int maxX;
        int maxY;
    };
}
#endif
