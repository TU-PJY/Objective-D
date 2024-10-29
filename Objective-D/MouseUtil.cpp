#include "MouseUtil.h"
#include "CameraUtil.h"

// 커서를 숨김 상태로 만든다
void MouseUtil::HideCursor() {
	::SetCursor(NULL);
}

// 마우스 모션 캡쳐를 시작한다
void MouseUtil::StartMotionCapture(HWND hWnd) {
	::SetCapture(hWnd);
	::GetCursorPos(&MotionPosition);
}

// 마우스 모셥 캡쳐를 중단한다
void MouseUtil::EndMotionCapture() {
	::ReleaseCapture();
}

// 마우스 모션 위치를 업데이트 한다
void MouseUtil::UpdateMotionPosition(POINT PrevPosition) {
	::SetCursorPos(PrevPosition.x, PrevPosition.y);
}

// 현재 커서의 위치를 얻는다
POINT MouseUtil::CurrentPosition() {
	POINT CursorPos;
	::GetCursorPos(&CursorPos);

	return CursorPos;
}

// 마우스 커서 위치를 뷰포트 위치로 변환한다.
void MouseUtil::UpdateMousePosition(HWND hWnd) {
	::GetCursorPos(&ClientPosition);
	::ScreenToClient(hWnd, &ClientPosition); // 클라이언트 좌표로 변환
	x = (((2.0f * (float)ClientPosition.x) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	y = -(((2.0f * (float)ClientPosition.y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
}