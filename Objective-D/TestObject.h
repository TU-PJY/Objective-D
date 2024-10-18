#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

#include "LineBrush.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	XMFLOAT3 Size{ 0.5, 0.5, 0.5 };
	float AlphaValue = 1.0;

	bool UseLight{};

	LineBrush line;

	OOBB oobb;

	Vector Vec{};

	TestObject() {
		Math::InitVector(Vec.Up, Vec.Right, Vec.Look);
		line.SetColor(1.0, 1.0, 1.0);
		Position.z = 3.0;
	}

	XMFLOAT3 GetUp() {
		return Vec.Up;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (mouse.LBUTTONDOWN && GetCapture() == hWnd) {
			mouse.HideCursor();
			GetCapture();

			float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
			mouse.SetPositionToPrev(PrevCursorPos);

			RotationDest.x += cyDelta;
			RotationDest.y += cxDelta;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_DOWN:
				if (AlphaValue > 0.0)
					AlphaValue -= 0.1;
				break;

			case VK_UP:
				if (AlphaValue < 1.0)
					AlphaValue += 0.1;
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

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			mouse.CaptureMotion(hWnd);
			break;

		case WM_LBUTTONUP:
			mouse.ReleaseMotion();
			break;
		}
	}

	void Update(float FT) {
		Rotation.x = std::lerp(Rotation.x, RotationDest.x, FT * 10);
		Rotation.y = std::lerp(Rotation.y, RotationDest.y, FT * 10);
		//camera.Rotate(Rotation.x * 0.01, Rotation.y * 0.01, 0.0);
	}

	void Render(CommandList CmdList) {
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Transform::Move(TranslateMatrix, Position.x, Position.y, 3.0);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, 0.0);

		if (!UseLight)
			DisableLight(CmdList);

		SetAlpha(CmdList, AlphaValue);
		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);

		// 바운드 박스 출력
		// OOBB의 경우 대상 모델을 렌더링한 직후 해야한다.
		// AABB, Range는 순서 무관
		oobb.Update(GunMesh, TranslateMatrix, RotateMatrix, ScaleMatrix);
		oobb.Render(CmdList);

		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		Transform::Move(TranslateMatrix, mouse.x, mouse.y, 0.0);
		BindTexture(CmdList, WoodTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);

		line.Draw(CmdList, 0.0, 0.0, mouse.x , mouse.y, 0.01);
	}
};