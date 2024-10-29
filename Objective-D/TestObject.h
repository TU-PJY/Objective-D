#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{0.0, 0.0, 3.0};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	float GunAlpha = 1.0;

	bool UseLight{};

	LineBrush line;

	OOBB oobb;
	Range range;

	Vector Vec{};

	TestObject() {
		Math::InitVector(Vec);
		line.SetColor(1.0, 1.0, 1.0);
		Position.z = 3.0;
	}

	XMFLOAT3 GetUp() {
		return Vec.Up;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}

	void InputMouseMotion(HWND hwnd, POINT MotionPosition) {
		if (GetCapture() == hwnd) {
			mouse.HideCursor();
			GetCapture();

			float cxDelta = (float)(mouse.CurrentPosition().x - MotionPosition.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - MotionPosition.y) / 5.0f;
			mouse.UpdateMotionPosition(MotionPosition);

			UpdateMotionRotation(RotationDest, cxDelta, cyDelta);
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_DOWN:
				if (GunAlpha > 0.0)
					GunAlpha -= 0.1;
				break;

			case VK_UP:
				if (GunAlpha < 1.0)
					GunAlpha += 0.1;
				break;

			case VK_SPACE:
				if (!UseLight)
					UseLight = true;
				else
					UseLight = false;
				break;
			}
			break;
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
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RENDER_TYPE_PERS);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Transform::Move(TranslateMatrix, Position.x, Position.y, Position.z);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, 0.0);
		FlipTexture(CmdList, FLIP_TYPE_V);

		if (!UseLight)
			DisableLight(CmdList);

		RenderMesh(CmdList, GunMesh, Tex, BasicShader, GunAlpha);

		// 바운드 스페어 출력
		range.Update(Position, 0.3);
		range.Render(CmdList);

		// 이미지 출력
		InitMatrix(CmdList, RENDER_TYPE_IMAGE);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		Transform::Move(TranslateMatrix, -0.5, -0.5, 0.0);
		RenderMesh(CmdList, ImagePannel, WoodTex, BasicShader);

		line.Draw(CmdList, 0.0, 0.0, mouse.x , mouse.y, 0.01);
	}
};