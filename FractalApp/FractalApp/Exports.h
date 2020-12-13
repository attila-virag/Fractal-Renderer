#pragma once
#include <string>

const std::string workingDirectory = "C:\\FractalAppData\\";

#ifdef _WIN32
// this class is exported by the FepProtocol DLL and imported
// by all other users
#if defined(DLL_EXPORTS)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define FEPPROTOCOL_SPEC
#endif
