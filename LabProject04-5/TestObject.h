#pragma once
#include "GameObject.h"
#include "MouseUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	bool FlipX{}, FlipY{};

	int FlipValue[2]{};

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (mouse.LBUTTONDOWN && GetCapture() == hWnd) {
			mouse.HideCursor();
			GetCapture();

			float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
			mouse.SetPositionToPrev(PrevCursorPos);

			Rotation.x += cyDelta;
			Rotation.y += cxDelta;
		}
	}
	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'H':
				if (!FlipX) {
					FlipValue[0] = 1;
					FlipX = true;
				}
				else {
					FlipValue[0] = 0;
					FlipX = false;
				}
				break;

			case 'V':
				if (!FlipY) {
					FlipValue[1] = 1;
					FlipY = true;
				}
				else {
					FlipValue[1] = 0;
					FlipY = false;
				}
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

	void Render(CommandList CmdList) {
		// 원근투영, 직각투영 중 하나를 선택하여 렌더링한다.
		//InitMatrix(CmdList, RenderType::Ortho);
		InitMatrix(CmdList, RenderType::Pers);

		Transform::Move(TranslateMatrix, Position.x, Position.y, 10.0);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y + 180.0, Rotation.z);

		FlipTexture(CmdList, FlipX, FlipY);

		// 텍스처 바인드 후 쉐이더를 적용한 후 매쉬를 렌더링한다.
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);
	}
};