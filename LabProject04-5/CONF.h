#pragma once
#include <tchar.h>
#include <string>

// layer configuration
enum class Layer 
{L1, L2, L3, END};

// camera mode configuration
enum class CamMode
{MODE1, MODE2, MODE3, MODE4, MODE5, MODE6};

// window name configuration
constexpr TCHAR D3D_WindowName[] = _T("Objective-D (");



constexpr int D3D_WindowNameLength = sizeof(D3D_WindowName) / sizeof(TCHAR);