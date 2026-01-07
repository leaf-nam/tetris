#pragma once

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

extern "C" {

	// C# 콜백 타입
	typedef void(*CallbackFunc)(int);

	// DLL에서 콜백 등록
	DLL_EXPORT void RegisterCallback(CallbackFunc cb);

	// DLL 루프 시작
	DLL_EXPORT void StartLoop();

	// DLL 루프 종료
	DLL_EXPORT void StopLoop();
}