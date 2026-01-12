#include "callback/callbacks.hpp"
#include "render/wpf_renderer.hpp"

WpfRenderer::WpfRenderer()
{
}

void WpfRenderer::set_callback(Callbacks* cbs)
{
	this->cbs = cbs;
}

void WpfRenderer::renderBackground() 
{
	this->cbs->background_callback();
}

void WpfRenderer::renderBoard(const Board& board, const Tetromino& tetromino) {
	int* board2D = new int[20 * 10];
	const uint16_t* rows = board.get_board();

	for (int y = 0; y < 20; ++y) {
		uint16_t row = rows[y + 2];

		for (int x = 0; x < 10; ++x) {
			board2D[y * 10 + x] = (row & (1 << x)) ? 1 : 0;
		}
	}

	this->cbs->board_callback(
		{ board2D, 10, 20 }, 
		{ tetromino.get_mino_type(), tetromino.get_rotation(), tetromino.get_pos().second, tetromino.get_pos().first}
	);
}

void WpfRenderer::renderTimer(const int sec) 
{
	this->cbs->timer_callback(sec);
}

void WpfRenderer::renderNextBlock(const int* tetromino_array) 
{
	//TetrominoWrapper t1 = map_to_tetromino(tetromino_array[0]);
	//TetrominoWrapper t2 = map_to_tetromino(tetromino_array[1]);
	//TetrominoWrapper t3 = map_to_tetromino(tetromino_array[2]);
	//this->cbs.next_block_callback(t1, t2, t3);
}

void WpfRenderer::renderHold(const Tetromino& tetromino) 
{
	//TetrominoWrapper tetromino_wrapper = map_to_tetromino(tetromino.get_mino_type());
	//this->cbs.hold_callback(tetromino_wrapper);
}

void WpfRenderer::renderScore(const int score) 
{
	//this->cbs.score_callback(score);
}

void WpfRenderer::renderLevel(const int level) 
{
	//this->cbs.level_callback(level);
}

WpfRenderer::~WpfRenderer() 
{
}