#pragma once
#include "GameObject.h"
#include "MouseUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	float AlphaValue = 1.0;

	bool UseLight{};

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

	void Render(CommandList CmdList) {
		//�� ��� ����
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Transform::Move(TranslateMatrix, Position.x, Position.y, 3.0);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y + 180.0, Rotation.z);

		if(!UseLight)
			DisableLight(CmdList);

		// �ؽ�ó ���ε� �� ���̴��� ������ �� �Ž��� �������Ѵ�.
		// �ʿ信 ���� �ؽ�ó �̹����� ������Ų��. ���� ��� (false, true), �̹����� ��� (true, true)
		// �̹����� ��� SetToImageMode()�Լ��� �����ϸ� �ڵ����� �˸°� �����ȴ�.
		SetAlpha(CmdList, AlphaValue);
		FlipTexture(CmdList, FlipHB, false, true, 0);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);
	}
};