#ifndef __CALLBACKS_HPP__
#define __CALLBACKS_HPP__

#include "tetromino_wrapper.hpp"
#include "board_wrapper.hpp"
#include <string>

using namespace std;

typedef char(*InputScanCallback)();
typedef void(*RenderBackgroundCallback)();
typedef void(*RenderBoardCallback)(BoardWrapper, TetrominoWrapper);
typedef void(*RenderTimerCallback)(int);
typedef void(*RenderNextBlockCallback)(const int*);
typedef void(*RenderHoldCallback)(int);
typedef void(*RenderScoreCallback)(int);
typedef void(*RenderLevelCallback)(int);
typedef void(*FinishCallback)();

struct Callbacks 
{
public:
	// Input용 콜백함수
	InputScanCallback scan_callback;

	// Render용 콜백함수
	RenderBackgroundCallback background_callback;
	RenderBoardCallback board_callback;
	RenderTimerCallback timer_callback;
	RenderNextBlockCallback next_block_callback;
	RenderHoldCallback hold_callback;
	RenderScoreCallback score_callback;
	RenderLevelCallback level_callback;
	
	// 게임 종료용 콜백함수
	FinishCallback finish_callback;
};

#endif