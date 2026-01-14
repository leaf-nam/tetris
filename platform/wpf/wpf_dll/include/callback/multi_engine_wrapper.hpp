#ifndef __ENGINE_WRAPPER_HPP__
#define __ENGINE_WRAPPER_HPP__

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

#include <cstdint>

extern "C"
{

	// DLL에서 콜백 등록
	DLL_EXPORT void register_multi_callbacks(
		InputScanCallback scan_callback,
		RenderBackgroundCallback background_callback,
		RenderBoardCallback board_callback,
		RenderHoldCallback hold_callback,
		RenderNextBlockCallback next_callback,
		RenderTimerCallback timer_callback,
		RenderScoreCallback score_callback,
		RenderLevelCallback level_callback,
		RenderOtherBoardCallback other_board_callback,
		FinishCallback finish_callback,
		SendUdpCallback send_callback,
		ReceiveUdpCallback receive_callback
	);

	// DLL 루프 시작
	DLL_EXPORT void init_multi_engine();

	// DLL 루프 종료
	DLL_EXPORT void run_multi_engine();

	DLL_EXPORT void stop_multi_engine();

	DLL_EXPORT void finish_multi_engine();
}
#endif