#include "Camera.h"
#include "Framework.h"


void Camera::Update(float FT) {
	if (RunningMode == CamMode::MODE1) {
		auto obj = fw.FindObject("obj1", LayerRange::Single, Layer::L1);
		if (obj) Track(obj->Position, obj, FT);
	}
	else if (RunningMode == CamMode::MODE2) {
		auto obj = fw.FindObject("obj2", LayerRange::Single, Layer::L2);
		if (obj) Track(obj->Position, obj, FT);
	}
	else if (RunningMode == CamMode::MODE3) {
		auto obj = fw.FindObject("obj3", LayerRange::Single, Layer::L2);
		if (obj) Track(obj->Position, obj, FT);
	}
	else if (RunningMode == CamMode::MODE4) {
		auto obj = fw.FindObject("obj4", LayerRange::Single, Layer::L2);
		if (obj) Track(obj->Position, obj, FT);
	}
	else if (RunningMode == CamMode::MODE5) {
		auto obj = fw.FindObject("obj5", LayerRange::Single, Layer::L2);
		if (obj) Track(obj->Position, obj, FT);
	}
	else if (RunningMode == CamMode::MODE6) {
		auto obj = fw.FindObject("obj6", LayerRange::Single, Layer::L2);
		if (obj) Track(obj->Position, obj, FT);
	}
}


void Camera::CameraKeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			SetCameraMode(CamMode::MODE1);
			break;

		case '2':
			SetCameraMode(CamMode::MODE2);
			break;

		case '3':
			SetCameraMode(CamMode::MODE3);
			break;

		case '4':
			SetCameraMode(CamMode::MODE4);
			break;

		case '5':
			SetCameraMode(CamMode::MODE5);
			break;

		case '6':
			SetCameraMode(CamMode::MODE6);
			break;

		}
		break;

	case WM_KEYUP:
		break;
	}
}