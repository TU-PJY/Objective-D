#include "MouseUtil.h"

void MouseUtil::HideCursor() {
	::SetCursor(NULL);
}

void MouseUtil::CaptureMotion(HWND hWnd) {
	::SetCapture(hWnd);
	::GetCursorPos(&PrevCursorPos);
}

void MouseUtil::ReleaseMotion() {
	::ReleaseCapture();
}

void MouseUtil::SetPositionToPrev(POINT PrevPosition) {
	::SetCursorPos(PrevPosition.x, PrevPosition.y);
}

POINT MouseUtil::CurrentPosition() {
	POINT CursorPos;
	::GetCursorPos(&CursorPos);

	return CursorPos;
}

void MouseUtil::UpdateButtonState(UINT nMessageID) {
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