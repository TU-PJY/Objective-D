#pragma once
#include <tchar.h>
#include <string>

// layer configuration
enum class D3D_Layer 
{L1, L2, L3, END};

// window name configuration
constexpr TCHAR D3D_WindowName[] = _T("Objective-D (");



constexpr int D3D_WindowNameLength = sizeof(D3D_WindowName) / sizeof(TCHAR);