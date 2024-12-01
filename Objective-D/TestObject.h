#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{0.0, 0.0, 0.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };

	LineBrush line;

	OOBB oobb;

	TestObject() {
		line.SetColor(1.0, 1.0, 1.0);
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
		}
	}

	void Update(float FT) {
		Rotation.x = std::lerp(Rotation.x, RotationDest.x, FT * 10);
		Rotation.y = std::lerp(Rotation.y, RotationDest.y, FT * 10);

		//camera.Rotate(Rotation.x, Rotation.y, Rotation.z);
	}

	void Render() {
		/*InitRenderState(RENDER_TYPE_3D);
		SetLightUse(DISABLE_LIGHT);
		Transform::Scale(ScaleMatrix, 20.0, 20.0, 20.0);
		Render3D(SkyboxMesh, SkyboxTex, 1.0, false);*/

		// 모델 출력
		InitRenderState(RENDER_TYPE_3D);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, 0.0);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		FlipTexture(FLIP_TYPE_V);
		Render3D(GunMesh, Tex);

		oobb.Update(GunMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
		oobb.Render();

		// 이미지 출력, 이미지 종횡비가 자동으로 적용된다.
		InitRenderState(RENDER_TYPE_2D);
		Transform::Move2D(TranslateMatrix, -0.5, 0.5);
		Transform::Scale2D(ScaleMatrix, 0.5, 0.5);
		Render2D(WoodTex);

		// 선 출력
		line.Draw(0.5, 0.0, mouse.x, mouse.y, 0.02);
	}
};