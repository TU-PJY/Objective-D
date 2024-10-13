#pragma once
#include "GameObject.h"
#include "MouseUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	bool FlipX{}, FlipY{};

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
				if (!FlipX)  FlipX = true;
				else  FlipX = false; 
				break;

			case 'V':
				if (!FlipY)  FlipY = true;
				else  FlipY = false;  
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
		// ��������, �������� �� �ϳ��� �����Ͽ� �������Ѵ�.
		//InitMatrix(CmdList, RenderType::Ortho);
		InitMatrix(CmdList, RenderType::Pers);

		Transform::Move(TranslateMatrix, Position.x, Position.y, 10.0);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y + 180.0, Rotation.z);

		// �ؽ�ó ���ε� �� ���̴��� ������ �� �Ž��� �������Ѵ�.
		// �ʿ信 ���� �ؽ�ó �̹����� ������Ų��. ���� ��� (false, true), �̹����� ��� (true, true)
		BindTexture(CmdList, Tex, false, true);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);
	}
};