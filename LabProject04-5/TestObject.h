#pragma once
#include "GameObject.h"
#include "MouseUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	float AlphaValue = 1.0;

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
			case VK_DOWN:
				if (AlphaValue > 0.0)
					AlphaValue -= 0.1;
				break;

			case VK_UP:
				if (AlphaValue < 1.0)
					AlphaValue += 0.1;
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
		//모델 출력 예시
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.2, 0.2, 0.2);
		Transform::Move(TranslateMatrix, Position.x, Position.y, 3.0);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y + 180.0, Rotation.z);

		// 텍스처 바인드 후 쉐이더를 적용한 후 매쉬를 렌더링한다.
		// 필요에 따라 텍스처 이미지를 반전시킨다. 모델의 경우 (false, true), 이미지의 경우 (true, true)
		SetAlpha(CmdList, AlphaHB[0], AlphaValue);
		FlipTexture(CmdList, FlipHB[0], false, true);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);


		// 이미지 출력 예시
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);

		// 이미지 출력 시 z값은 IMAGE_Z를 사용하는 것을 권장함
		Transform::Move(TranslateMatrix, 0.0, 0.0, IMAGE_Z);
		SetAlpha(CmdList, AlphaHB[1], 0.5);
		BindTexture(CmdList, WoodTex);
		UseShader(CmdList, BasicShader);

		// 이미지 출력 전용 매쉬를 사용한다.
		RenderMesh(CmdList, ImagePannel);
	}
};