#pragma once
#include "FrameworkUtil.h"

namespace Mode1 {
	void Start();
	void Destructor();
	void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseMotionController(HWND hWnd);
	void MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
}