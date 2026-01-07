#pragma once

#ifdef _WIN32
#ifdef DLL_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_API
#endif

extern "C" {
}