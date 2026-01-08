#include "render/wpf_renderer.hpp"
#include "callbacks.hpp"

BoardWrapper map_to_board(const Board& board) {
	BoardWrapper boardWrapper;
	return boardWrapper;
}

TetrominoWrapper map_to_tetromino(const Tetromino& tetromino) {
	TetrominoWrapper tetrominoWrapper;
	return tetrominoWrapper;
}

WpfRenderer::WpfRenderer()
{
}

void WpfRenderer::render_background() 
{
	this->cbs.background_callback();
}

void WpfRenderer::render_board(const Board& board, const Tetromino& tetromino) 
{
	BoardWrapper board_wrapper = map_to_board(board);
	TetrominoWrapper tetromino_wrapper = map_to_tetromino(tetromino);
	this->cbs.board_callback(board_wrapper, tetromino_wrapper);
}

void WpfRenderer::render_timer(const int sec) 
{
	this->cbs.timer_callback(sec);
}

void WpfRenderer::render_next_block(const Tetromino* tetromino_array) 
{
	TetrominoWrapper t1 = map_to_tetromino(tetromino_array[0]);
	TetrominoWrapper t2 = map_to_tetromino(tetromino_array[1]);
	TetrominoWrapper t3 = map_to_tetromino(tetromino_array[2]);
	this->cbs.next_block_callback(t1, t2, t3);
}

void WpfRenderer::render_hold(const Tetromino& tetromino) 
{
	TetrominoWrapper tetromino_wrapper = map_to_tetromino(tetromino);
	this->cbs.hold_callback(tetromino_wrapper);
}

void WpfRenderer::render_score(const int score) 
{
	this->cbs.score_callback(score);
}

void WpfRenderer::render_level(const int level) 
{
	this->cbs.level_callback(level);
}

void WpfRenderer::set_callback(Callbacks cbs) 
{
	this->cbs = cbs;
}

WpfRenderer::~WpfRenderer() 
{
}