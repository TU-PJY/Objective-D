#pragma once
#include "GameObject.h"
#include "MouseUtil.h"

class TestObject : public GameObject {
public:
	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};
	bool FlipX{}, FlipY{};
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
			case 'H':
				if (!FlipX)  FlipX = true;
				else  FlipX = false; 
				break;

			case 'V':
				if (!FlipY)  FlipY = true;
				else  FlipY = false;  
				break;

			case VK_DOWN:
				if(AlphaValue > 0.0)
					AlphaValue -= 0.1;
				break;

			case VK_UP:
				if(AlphaValue < 1.0)
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
		// �� ��� ����
		//InitMatrix(CmdList, RenderType::Pers);
		//Transform::Move(TranslateMatrix, Position.x, Position.y, 1.0);
		//Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y + 180.0, Rotation.z);

		//// �ؽ�ó ���ε� �� ���̴��� ������ �� �Ž��� �������Ѵ�.
		//// �ʿ信 ���� �ؽ�ó �̹����� ������Ų��. ���� ��� (false, true), �̹����� ��� (true, true)
		//SetAlpha(CmdList, AlphaHB[0], AlphaValue);
		//FlipTexture(CmdList, FlipHB[0], false, true);
		//BindTexture(CmdList, Tex);
		//UseShader(CmdList, BasicShader);
		//RenderMesh(CmdList, GunMesh);

		// �̹��� ��� ����
		InitMatrix(CmdList, RenderType::Ortho);
		SetToImageMode(CmdList);

		// �̹��� ��� �� z���� 1.0�� ����
		Transform::Move(TranslateMatrix, 0.0, 0.0, 1.0);
		SetAlpha(CmdList, AlphaHB[1], 1.0);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);

		// �̹��� ��� ���� �Ž��� ����Ѵ�.
		RenderMesh(CmdList, ImagePannel);
	}
};