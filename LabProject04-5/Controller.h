#pragma once
#include "FrameworkUtil.h"
#include "MouseUtil.h"

// ��帶�� ������ ���ӽ����̽�
namespace Mode_1 {
	inline void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (auto obj = framework.Find("obj1"); obj)
			obj->InputKey(hWnd, nMessageID, wParam, lParam);

		// end program
		if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
			framework.Exit();
	}

	inline void MouseMotionController(HWND hWnd) {
		if (auto obj = framework.Find("obj1"); obj)
			obj->InputMouseMotion(hWnd, mouse.PrevCursorPos);
	}

	inline void MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		if (auto obj = framework.Find("obj1"); obj)
			obj->InputMouseButton(hWnd, nMessageID, wParam, lParam);

		mouse.UpdateButtonState(nMessageID);
	}
}