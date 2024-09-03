#pragma once
#include "DirectX_3D.h"

enum class Type
{LEFT_BUTTON, RIGHT_BUTTON};

enum class State
{BUTTON_DOWN, BUTTON_UP};

class MouseUtil {
public:
	float x{}, y{};
	POINT PrevCursorPos{};
	bool LBUTTONDOWN{}, RBUTTONDOWN{};

	void HideCursor() {
		::SetCursor(NULL);
	}

	void CaptureMotion(HWND hWnd) {
		::SetCapture(hWnd);
		::GetCursorPos(&PrevCursorPos);
	}

	void ReleaseMotion() {
		::ReleaseCapture();
	}

	void SetPositionToPrev(POINT PrevPosition) {
		::SetCursorPos(PrevPosition.x, PrevPosition.y);
	}

	POINT CurrentPosition() {
		POINT CursorPos;
		::GetCursorPos(&CursorPos);

		return CursorPos;
	}

	void UpdateButtonState(UINT nMessageID) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			LBUTTONDOWN = true;
			break;

		case WM_LBUTTONUP:
			LBUTTONDOWN = false;
			break;

		case WM_RBUTTONDOWN:
			RBUTTONDOWN = true;
			break;

		case WM_RBUTTONUP:
			RBUTTONDOWN = false;
			break;
		}
	}
};
extern MouseUtil mouse;
