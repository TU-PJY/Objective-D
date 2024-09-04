#pragma once
#include <tchar.h>
#include <string>

// layer configuration
enum class Layer
{ Terrain, L1, L2, END };

// camera mode configuration
enum class CamMode
{ MODE1 };

// full screen option
constexpr bool START_WITH_FULL_SCREEN = true;

// window name configuration
constexpr TCHAR WindowName[] = _T("Objective-D (");
constexpr int WindowNameLength = sizeof(WindowName) / sizeof(TCHAR);