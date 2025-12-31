#include <iostream>
#include "board/board.h"
#include "block/block.h"

using namespace std;

const double FPS = 30.0;

int main(void) {
	Board board;

	board.set(3, 4);
	
	board.print();

	cout << "exits??" << board.exist(3, 4) << endl;

	BlockData block(1);

	cout << "type : " << block.type << endl;
	cout << "r : " << block.r << endl;
	cout << "c : " << block.c << endl;
	cout << "turn : " << block.turn << endl;

	block.turn_right();
	block.turn_right();
	block.turn_right();

	cout << "turn : " << block.turn << endl;

	block.turn_right();
	block.turn_right();
	cout << "turn : " << block.turn << endl;

	block.turn_left();
	cout << "turn : " << block.turn << endl;
	block.turn_left();
	cout << "turn : " << block.turn << endl;
	block.turn_left();
	cout << "turn : " << block.turn << endl;

	for (int i = 0; i < 7; i++) {
		cout << "==========[" << i << "]==========" << endl;
		BlockData block(i);
		for (int j = 0; j < 4; j++) {
			cout << "<<<<(turn left)<<<<" << endl;
			block.print();
			block.turn_left();
		}
		
	}

	getchar();

	return 0;
}