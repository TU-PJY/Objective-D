#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>
#include "PickingUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{0.0, 0.0, 0.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	XMFLOAT3 Color{ 0.0, 0.0, 0.0 };

	LineBrush line;

	OOBB oobb;

	PickingUtil pickUtil;
	
	float point = 0.1;

	TestObject() {
		line.SetColor(1.0, 1.0, 1.0);
		camera.Move(XMFLOAT3(0.0, 0.0, -2.0));
		Position.z = 5.0;
	}

	void InputMouseMotion(HWND hwnd, POINT MotionPosition) {
		if (GetCapture() == hwnd) {
			mouse.HideCursor();
			GetCapture();

			XMFLOAT2 Delta = mouse.GetMotionDelta(MotionPosition, 0.5);
			mouse.UpdateMotionPosition(MotionPosition);
			UpdateMotionRotation(RotationDest, Delta.x, Delta.y);
		}
	}

	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			mouse.StartMotionCapture(hWnd);
			break;

		case WM_LBUTTONUP:
			mouse.EndMotionCapture();
			break;

		case WM_RBUTTONDOWN:
			if (pickUtil.PickByCursor(lParam, this, GunMesh)) {
				Color = XMFLOAT3(1.0, 0.0, 0.0);
			}
			else
				Color = XMFLOAT3(0.0, 0.0, 0.0);
			break;
		}
	}

	void Update(float FT) {
		Rotation.x = std::lerp(Rotation.x, RotationDest.x, FT * 10);
		Rotation.y = std::lerp(Rotation.y, RotationDest.y, FT * 10);
	}

	void Render() override {
		/*InitRenderState(RENDER_TYPE_3D);
		SetLightUse(DISABLE_LIGHT);
		Transform::Scale(ScaleMatrix, 20.0, 20.0, 20.0);
		Render3D(SkyboxMesh, SkyboxTex, 1.0, false);*/

		// 모델 출력
		InitRenderState(RENDER_TYPE_3D);
		SetColor(Color);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, 0.0);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Render3D(GunMesh, Tex);

		// 피킹 행렬 업데이트, 렌더링 직후 실행할 것을 추천
		UpdatePickMatrix();

		oobb.Update(GunMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
		oobb.Render();

		// 이미지 출력, 이미지 종횡비가 자동으로 적용된다.
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move2D(TranslateMatrix, -0.5, 0.5);
		Transform::Scale2D(ScaleMatrix, 0.5, 0.5);
		Render2D(WoodTex);
	}
};