#include "MouseUtil.h"
#include "CameraUtil.h"

// 커서를 숨김 상태로 만든다
void MouseUtil::HideCursor() {
	::SetCursor(NULL);
}

// 마우스 모션 캡쳐를 시작한다
void MouseUtil::CaptureMotion(HWND hWnd) {
	::SetCapture(hWnd);
	::GetCursorPos(&PrevCursorPos);
}

// 마우스 모셥 캡쳐를 중단한다
void MouseUtil::ReleaseMotion() {
	::ReleaseCapture();
}

// 마우스 커서 위치를 이전 위치로 이동시킨다
void MouseUtil::SetPositionToPrev(POINT PrevPosition) {
	::SetCursorPos(PrevPosition.x, PrevPosition.y);
}

// 현재 커서의 위치를 얻는다
POINT MouseUtil::CurrentPosition() {
	POINT CursorPos;
	::GetCursorPos(&CursorPos);

	return CursorPos;
}

void MouseUtil::UpdateMousePosition(HWND hWnd) {
	::GetCursorPos(&ClientPosition);
	::ScreenToClient(hWnd, &ClientPosition); // 클라이언트 좌표로 변환
	x = (((2.0f * (float)ClientPosition.x) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	y = -(((2.0f * (float)ClientPosition.y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
}

// 마우스 버튼 클릭 상태를 업데이트 한다
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