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
	float AlphaValue = 1.0;

	bool UseLight{};

	LineBrush line;

	Vector Vec{};

	TestObject() {
		Math::InitVector(Vec.Up, Vec.Right, Vec.Look);
		line.SetColor(0.0, 1.0, 0.0);
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

	}

	void Render(CommandList CmdList) {
		// ��ī�̹ڽ� ��� ����
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 5.0, 5.0, 5.0);
		camera.Rotate(Rotation.x * 0.01, Rotation.y * 0.01, 0.0);

		// ��ī�̹ڽ� ��� �� ������ ��Ȱ��ȭ �ؾ��Ѵ�.
		DisableLight(CmdList);

		// ��ī�̹ڽ� ���� �Ž��� �ؽ�ó�� ����ؾ� �Ѵ�.
		BindTexture(CmdList, SkyboxTex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, SkyboxMesh);

		//�� ��� ����
		InitMatrix(CmdList, RenderType::Pers);
		Transform::Scale(ScaleMatrix, 0.4, 0.4, 0.4);
		Transform::Move(TranslateMatrix, Position.x, Position.y, 3.0);
		Transform::Rotate(RotateMatrix, 0.0, 90.0, 0.0);

		if (!UseLight)
			DisableLight(CmdList);

		// �ؽ�ó ���ε� �� ���̴��� ������ �� �Ž��� �������Ѵ�.
		// �ʿ信 ���� �ؽ�ó �̹����� ������Ų��. �𵨿� ���� �ɹ���
		// �̹����� ��� SetToImageMode()�Լ��� �����ϸ� �ڵ����� �˸°� �����ȴ�.
		SetAlpha(CmdList, AlphaValue);
		FlipTexture(CmdList, false, true);
		BindTexture(CmdList, Tex);
		UseShader(CmdList, BasicShader);
		RenderMesh(CmdList, GunMesh);


		// �̹��� ���, ���̰˻縦 ���� �ʱ� ������ �ǵ��� �ֻ��� ���̾�� ��µǵ��� �ϴ°��� �����Ѵ�.
		InitMatrix(CmdList, RenderType::Ortho);

		// �̹��� ��� �� �ݵ�� �̹��� ���� ��ȯ�ؾ��Ѵ�.
		SetToImageMode(CmdList);
		
		Transform::Scale(ScaleMatrix, 0.5, 0.5, 1.0);
		Transform::Move(TranslateMatrix, -0.5, -0.5, 0.0);
		BindTexture(CmdList, WoodTex);

		// ���� �˻縦 ��Ȱ��ȭ �ؾ��Ѵ�.
		UseShader(CmdList, BasicShader, false);
		RenderMesh(CmdList, ImagePannel);

		// ���� �׸���.
		line.Draw(CmdList, -0.55, -0.12, mouse.x, mouse.y, 0.02, 1.0);
	}
};