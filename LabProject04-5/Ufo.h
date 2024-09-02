#pragma once
#include "ObjectBase.h"
#include <random>

class Aircraft : public BASE {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};
	float SpeedForward{};
	float SpeedStrafe{};

	Mesh* ObjectMesh;
	Shader* Shader;

	XMFLOAT3 ObjPosition{};
	XMFLOAT3 Rotation{};

	BoundingOrientedBox OOBB = BoundingOrientedBox();

public:
	Aircraft() {
		SetShader(Shader, pseudoShader);
		SetMesh(ObjectMesh, "pFlyerMesh");
	}

	
	XMFLOAT3 GetPosition() {
		return ObjPosition;
	}

	void InputNewPosition(float X, float Y, float Z) {
		ObjPosition.x = X;
		ObjPosition.y = Y;
		ObjPosition.z = Z;
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

		MoveForward(ObjPosition, SpeedForward);
		MoveStrafe(ObjPosition, SpeedStrafe);
	}

	void Update(float FT) {
		MoveAircraft(FT);

		if(auto ptr = framework.Find("map"); ptr)
			framework.CheckCollisionTerrain(ObjPosition, ptr);

		UpdateOOBB(OOBB, ObjectMesh);
	}

	void Render(CommandList CmdList) {
		BeginProcess();
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));
		SetPosition(ObjPosition);
		Rotate(Rotation.x, Rotation.y, Rotation.z);
		RenderMesh(CmdList, Shader, ObjectMesh);

		BeginProcess();
		SetPosition(0.0, 50.0, 0.0);
		RenderMesh(CmdList, Shader, ObjectMesh);
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

			//UpdateRotation(cyDelta, cxDelta, 0.0);
			Rotation.x += cyDelta;
			Rotation.y += cxDelta;
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
		RenderMesh(CmdList, Shader, TerrainMesh);
	}

	void InputMouseMotion(POINT CursorPos, POINT PrevCursorPos, bool LButtonDownState, bool RButtonDownState) {}

	void InputMouseButton(POINT CursorPos, bool LButtonDownState, bool RButtonDownState) {}
};