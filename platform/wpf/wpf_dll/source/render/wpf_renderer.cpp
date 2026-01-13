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
	auto board_grid = board.get_board();

	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 10; ++x) {
			board2D[y * 10 + x] = board_grid[y + 2][x];
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
	this->cbs->next_block_callback(tetromino_array);
}

void WpfRenderer::renderHold(const Tetromino& tetromino) 
{
	this->cbs->hold_callback(tetromino.get_mino_type());
}

void WpfRenderer::renderScore(const int score) 
{
	this->cbs->score_callback(score);
}

void WpfRenderer::renderLevel(const int level) 
{
	this->cbs->level_callback(level);
}

WpfRenderer::~WpfRenderer() 
{
}