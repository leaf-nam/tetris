#ifndef __TETROMINOQUEUE_HPP__
#define __TETROMINOQUEUE_HPP__

class TetrominoQueue
{
    private:
    int tetrominos[7];
    TetrominoQueue();

    public:
    static TetrominoQueue& get_instance();
	TetrominoQueue(const TetrominoQueue&) = delete;
	TetrominoQueue& operator=(const TetrominoQueue&) = delete;
    int get_new_tetromino();
    void set_new_tetromino(int new_tetromino);
    const int* get_tetrominos();
    void draw_tetromino_queue();
};

#endif
