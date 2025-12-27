#include "board/board.hpp"
#include "input/action.hpp"

#include <thread>
#include <chrono>

using namespace std;

int main(void) {
    Board board;
    
    board.spawn_mino(0);
    board.draw_board();
    this_thread::sleep_for(chrono::milliseconds(1000));
    board.move_mino(Action::DROP);
    board.update_board();
    board.draw_board();

    this_thread::sleep_for(chrono::milliseconds(1000));
    board.move_mino(Action::DROP);
    board.update_board();
    board.draw_board();

    this_thread::sleep_for(chrono::milliseconds(1000));
    board.move_mino(Action::DROP);
    board.update_board();
    board.draw_board();

    this_thread::sleep_for(chrono::milliseconds(1000));
    board.move_mino(Action::DROP);
    board.update_board();
    board.draw_board();

    return 0;
}