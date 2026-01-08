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
	DLL_EXPORT void register_callback(const Callbacks cbs);

	// DLL 루프 시작
	DLL_EXPORT void init_engine();

	// DLL 루프 종료
	DLL_EXPORT void run_engine();

	DLL_EXPORT void stop_engine();

	DLL_EXPORT void finish_engine();
}
#endif