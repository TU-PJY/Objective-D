#pragma once
#include "FrameworkUtil.h"

// ��帶�� ������ ���ӽ����̽�
namespace Mode_1 {
	inline void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN: case WM_KEYUP:
		{
			if (auto obj = fw.Find("obj1"); obj)
				obj->ObjectKeyboardController(nMessageID, wParam);
		}

		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(1);
			break;
		}

		break;
		}
	}

	inline void MouseMotionController(HWND hwnd) {
		if (GetCapture() == hwnd) {
			if(auto obj = fw.Find("obj1"); obj)
				obj->ObjectMouseMotionController(fw.PrevCursorPosition, fw.LButtonDownState, fw.RButtonDownState);
		}
	}

	inline void MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	}
}