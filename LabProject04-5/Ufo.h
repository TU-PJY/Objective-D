#pragma once
#include "ObjectBase.h"
#include <random>

class Aircraft : public BASE {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};
	float SpeedForward{};
	float SpeedStrafe{};

	Mesh* Mesh;
	Shader* Shader;

public:
	Aircraft() {
		SetShader(Shader, pseudoShader);
		SetMesh(Mesh, "pFlyerMesh");
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));
	}

	void MoveAircraft(float FT) {
		if (MoveFront)
			LerpAcc(SpeedForward, 0.15, 5, FT);
		if (MoveBack)
			LerpAcc(SpeedForward, -0.15, 5, FT);
		if (MoveRight)
			LerpAcc(SpeedStrafe, 0.15, 5, FT);
		if (MoveLeft)
			LerpAcc(SpeedStrafe, -0.15, 5, FT);

		if (!MoveFront && !MoveBack)
			LerpDcc(SpeedForward, 5, FT);
		if (!MoveRight && !MoveLeft)
			LerpDcc(SpeedStrafe, 5, FT);

		MoveForward(SpeedForward);
		MoveStrafe(SpeedStrafe);
	}

	void Update(float FT) {
		InitTransform();

		MoveAircraft(FT);

		SetPosition(Position);
		Rotate(Rotation.x, Rotation.y, Rotation.z);

		auto ptr = framework.Find("map");
		if (ptr) framework.CheckCollisionTerrain(this, ptr);

		UpdateOOBB();
	}

	void Render(CommandList CmdList) {
		RenderShader(CmdList, Shader);
		RenderMesh(CmdList, Mesh);
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'W':
				MoveFront = true;
				break;

			case 'S':
				MoveBack = true;
				break;

			case 'D':
				MoveRight = true;
				break;

			case 'A':
				MoveLeft = true;
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W':
				MoveFront = false;
				break;

			case 'S':
				MoveBack = false;
				break;

			case 'D':
				MoveRight = false;
				break;

			case 'A':
				MoveLeft = false;
				break;
			}
			break;
		}
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (framework.LB_DownState && GetCapture() == hWnd) {
			::SetCursor(NULL);

			float cxDelta = 0.0;
			float cyDelta = 0.0;

			cxDelta = (float)(framework.NewCursorPos().x - PrevCursorPos.x) / 5.0f;
			cyDelta = (float)(framework.NewCursorPos().y - PrevCursorPos.y) / 5.0f;
			::SetCursorPos(PrevCursorPos.x, PrevCursorPos.y);

			UpdateRotation(cyDelta, cxDelta, 0.0);
		}
	}

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			framework.CaptureMouseMotion(hWnd);
			framework.UpdateMouseButton(ButtonType::LButton, ButtonState::Down);
			break;

		case WM_RBUTTONDOWN:
			framework.CaptureMouseMotion(hWnd);
			framework.UpdateMouseButton(ButtonType::RButton, ButtonState::Down);
			break;

		case WM_LBUTTONUP:
			framework.ReleaseMouseMotion();
			framework.UpdateMouseButton(ButtonType::LButton, ButtonState::Up);
			break;

		case WM_RBUTTONUP:
			framework.ReleaseMouseMotion();
			framework.UpdateMouseButton(ButtonType::RButton, ButtonState::Up);
			break;
		}
	}
};

class Map : public BASE {
private:
	Mesh* TerrainMesh;
	Shader* Shader;

public:
	Mesh* GetTerrainMesh() {
		return TerrainMesh;
	}

	Map() {
		SetShader(Shader, pseudoShader);
		SetTerrain(TerrainMesh, "pTerrain");
		SetColor(XMFLOAT3(0.133333, 0.545098, 0.133333));
		Scale(5.0, 5.0, 5.0);
	}

	void InputKey(UINT nMessageID, WPARAM wParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			break;

		case WM_KEYUP:
			break;
		}
	}

	void Render(CommandList CmdList) {
		RenderShader(CmdList, Shader);
		RenderMesh(CmdList, TerrainMesh);
	}

	void InputMouseMotion(POINT CursorPos, POINT PrevCursorPos, bool LButtonDownState, bool RButtonDownState) {}

	void InputMouseButton(POINT CursorPos, bool LButtonDownState, bool RButtonDownState) {}
};