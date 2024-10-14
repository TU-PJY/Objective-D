#include "MouseUtil.h"

// Ŀ���� ���� ���·� �����
void MouseUtil::HideCursor() {
	::SetCursor(NULL);
}

// ���콺 ��� ĸ�ĸ� �����Ѵ�
void MouseUtil::CaptureMotion(HWND hWnd) {
	::SetCapture(hWnd);
	::GetCursorPos(&PrevCursorPos);
}

// ���콺 ��� ĸ�ĸ� �ߴ��Ѵ�
void MouseUtil::ReleaseMotion() {
	::ReleaseCapture();
}

// ���콺 Ŀ�� ��ġ�� ���� ��ġ�� �̵���Ų��
void MouseUtil::SetPositionToPrev(POINT PrevPosition) {
	::SetCursorPos(PrevPosition.x, PrevPosition.y);
}

// ���� Ŀ���� ��ġ�� ��´�
POINT MouseUtil::CurrentPosition() {
	POINT CursorPos;
	::GetCursorPos(&CursorPos);

	return CursorPos;
}

// ���콺 ��ư Ŭ�� ���¸� ������Ʈ �Ѵ�
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