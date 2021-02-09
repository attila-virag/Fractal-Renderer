#pragma once
#include <string>

const std::string workingDirectory = "C:\\FractalAppData\\";

#ifdef _WIN32
#if defined(DLL_EXPORTS)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#endif
