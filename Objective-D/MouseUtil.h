#pragma once
#include "DirectX_3D.h"

// 마우스를 담당하는 유틸이다. 

enum class Type
{LEFT_BUTTON, RIGHT_BUTTON};

enum class State
{BUTTON_DOWN, BUTTON_UP};

class MouseUtil {
public:
	float x{}, y{};
	POINT PrevCursorPos{};
	POINT ClientPosition{};
	bool LBUTTONDOWN{}, RBUTTONDOWN{};

	void HideCursor();
	void CaptureMotion(HWND hWnd);
	void ReleaseMotion();
	void SetPositionToPrev(POINT PrevPosition);
	POINT CurrentPosition();
	void UpdateMousePosition(HWND hWnd);
	void UpdateButtonState(UINT nMessageID);
};
extern MouseUtil mouse;
