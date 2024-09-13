#pragma once
#include "FrameworkUtil.h"

namespace Mode_1 {
	const char* Mode1();
	void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseMotionController(HWND hWnd);
	void MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void SetController();
}