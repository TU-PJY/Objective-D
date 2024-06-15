#include "Camera.h"
#include "Framework.h"


void Camera::Update(float FT) {
	auto obj = fw.FindObject("obj1", LayerRange::Single, LayerFW::L1);
	if (obj) TrackCamera(obj->Position, obj, FT);
}


void Camera::CameraController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_KEYDOWN:
		break;

	case WM_KEYUP:
		break;
	}
}