#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "CameraUtil.h"
#include <cmath>

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	XMFLOAT3 RotationDest{};
	float AlphaValue = 1.0;

	bool UseLight{};

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
	}

	void Render(CommandList CmdList) {
		// 스카이박스 출력 예시
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 5.0, 5.0, 5.0);
		camera.Rotate(Rotation.x * 0.01, Rotation.y * 0.01, 0.0);

		// 스카이박스 출력 시 조명을 비활성화 해야한다.
		DisableLight(CmdList);

		// 스카이박스 전용 매쉬와 텍스처를 사용해야 한다.
		BindTexture(CmdList, SkyboxTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, SkyboxMesh);

		//모델 출력 예시
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Transform::Move(TranslateMatrix, Position.x, Position.y, 3.0);
		Transform::Rotate(RotateMatrix, 0.0, 90.0, 0.0);

		if (!UseLight)
			DisableLight(CmdList);

		// 텍스처 바인드 후 쉐이더를 적용한 후 매쉬를 렌더링한다.
		// 필요에 따라 텍스처 이미지를 반전시킨다. 모델에 따라 케바케
		// 이미지의 경우 SetToImageMode()함수를 실행하면 자동으로 알맞게 반전된다.
		SetAlpha(CmdList, AlphaValue);
		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);


		// 이미지 출력, 깊이검사를 하지 않기 때문에 되도록 최상위 레이어에서 출력되도록 하는것을 권장한다.
		InitMatrix(CmdList, RenderType::Ortho);

		// 이미지 출력 시 반드시 이미지 모드로 전환해야한다.
		SetToImageMode(CmdList);
		
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		BindTexture(CmdList, WoodTex);
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);
	}
};