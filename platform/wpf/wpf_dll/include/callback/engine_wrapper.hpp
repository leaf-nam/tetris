#ifndef __ENGINE_WRAPPER_HPP__
#define __ENGINE_WRAPPER_HPP__

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <cstdint>
#include "engine_wrapper.hpp"

extern "C" 
{

	// DLL에서 콜백 등록
	DLL_EXPORT void register_callbacks(
		InputScanCallback scan_callback,
		RenderBackgroundCallback background_callback,
		RenderBoardCallback board_callback,
		RenderHoldCallback hold_callback,
		RenderNextBlockCallback next_callback,
		RenderTimerCallback timer_callback,
		RenderScoreCallback score_callback,
		RenderLevelCallback level_callback,
		FinishCallback finish_callback
	);

	// DLL 루프 시작
	DLL_EXPORT void init_engine();

	// DLL 루프 종료
	DLL_EXPORT void run_engine();

	DLL_EXPORT void stop_engine();

	DLL_EXPORT void finish_engine();
}
#endif