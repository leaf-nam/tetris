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
	void register_callbacks(
		const char* (*scan_callback)(),
		void (*background_callback)(int),
		void (*board_callback)(int),
		void (*timer_callback)(int),
		void (*nextblock_callback)(int),
		void (*hold_callback)(int),
		void (*score_callback)(int),
		void (*level_callback)(int)
	);

	// DLL 루프 시작
	DLL_EXPORT void init_engine();

	// DLL 루프 종료
	DLL_EXPORT void run_engine();

	DLL_EXPORT void stop_engine();

	DLL_EXPORT void finish_engine();
}
#endif