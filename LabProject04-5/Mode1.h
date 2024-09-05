#pragma once
#include "FrameworkUtil.h"
#include <string>

namespace Mode_1 {
	std::string Mode1();
	void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseMotionController(HWND hWnd);
	void MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
}