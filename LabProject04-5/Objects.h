#pragma once
#include "GameObject.h"
#include "MouseUtil.h"
#include "FrameworkUtil.h"
#include "PickingUtil.h"
#include "TerrainUtil.h"
#include "CollisionUtil.h"
#include <iostream>

class Aircraft : public GameObject {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};
	float SpeedForward{};
	float SpeedStrafe{};

	Mesh* ObjectMesh;
	Shader* Shader;

	XMFLOAT3 ObjPosition{};
	XMFLOAT3 Rotation{};
	ObjectVector v;

	PickingUtil pu;
	OOBB oobb;

public:
	Aircraft() {
		SetShader(Shader, pseudoShader);
		SetMesh(ObjectMesh, "pFlyerMesh");
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));
		Math::InitVector(v.Up, v.Right, v.Look);
	}

	XMFLOAT3 GetUp() {
		return v.Up;
	}

	XMFLOAT3 GetRight() {
		return v.Right;
	}

	XMFLOAT3 GetLook() {
		return v.Look;
	}

	Mesh* GetObjectMesh() {
		return ObjectMesh;
	}

	XMFLOAT3 GetPosition() {
		return ObjPosition;
	}

	OOBB GetOOBB() {
		return oobb;
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

		MoveForward(ObjPosition, v.Look, SpeedForward);
		MoveStrafe(ObjPosition, v.Right, SpeedStrafe);
	}

	void Update(float FT) {
		MoveAircraft(FT);
		terrainUtil.CheckCollision(ObjPosition);
		oobb.Update(ObjPosition, XMFLOAT3(3.0, 1.0, 8.0), Rotation);
		if (auto object = framework.Find("obj2"); object && oobb.CheckCollision(object->GetOOBB()))
			object->SetColor(1.0, 0.0, 0.0);


		Math::UpdateVector(v.Up, v.Right, v.Look, Rotation.x, Rotation.y, Rotation.z);
		std::cout << GetUp().x << " " << GetUp().y << " " << GetUp().z << std::endl;
	}

	void Render(CommandList CmdList) {
		BeginProcess();
		Transform::Scale(ScaleMatrix, 1.0, 1.0, 1.0);
		Transform::Move(TranslateMatrix, ObjPosition.x, ObjPosition.y, ObjPosition.z);
		Transform::Rotate(RotateMatrix, Rotation.x, Rotation.y, Rotation.z);
		RenderMesh(CmdList, Shader, ObjectMesh);
	}

	void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {
		if (mouse.LBUTTONDOWN && GetCapture() == hWnd) {
			mouse.HideCursor();

			float cxDelta = (float)(mouse.CurrentPosition().x - PrevCursorPos.x) / 5.0f;
			float cyDelta = (float)(mouse.CurrentPosition().y - PrevCursorPos.y) / 5.0f;
			mouse.SetPositionToPrev(PrevCursorPos);

			Rotation.x += cyDelta;
			Rotation.y += cxDelta;
		}
	}

	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_LBUTTONDOWN:
			mouse.CaptureMotion(hWnd);
			break;

		case WM_RBUTTONDOWN:
			if (auto object = framework.Find("obj2"); object && pu.PickByCursor(lParam, object, object->GetObjectMesh()))
				object->SetColor(0.0, 1.0, 0.0);
			break;

		case WM_LBUTTONUP:
			mouse.ReleaseMotion();
			break;
		}
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case 'W': MoveFront = true; break;
			case 'S': MoveBack = true; break;
			case 'A': MoveLeft = true; break;
			case 'D': MoveRight = true; break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case 'W': MoveFront = false; break;
			case 'S': MoveBack = false; break;
			case 'A': MoveLeft = false; break;
			case 'D': MoveRight = false; break;
			}
			break;
		}
	}
};


class Object : public GameObject {
public:
	Mesh* ObjectMesh;
	Shader* ObjectShader;

	OOBB oobb;

	OOBB GetOOBB() {
		return oobb;
	}

	Mesh* GetObjectMesh() {
		return ObjectMesh;
	}

	Object() {
		SetMesh(ObjectMesh, "pUfoMesh");
		SetShader(ObjectShader, pseudoShader);
		SetColor(XMFLOAT3(0.0, 1.0, 0.0));
	}

	void Update(float FT) {
		oobb.Update(XMFLOAT3(-50.0, 50.0, -10.0), XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3(0.0, 0.0, 0.0));
	}

	void Render(CommandList CmdList) {
		BeginProcess();
		Transform::Scale(ScaleMatrix, 1.0, 1.0, 1.0);
		Transform::Move(TranslateMatrix, -50.0, 50.0, -10.0);
		RenderMesh(CmdList, ObjectShader, ObjectMesh);
	}
};


class Map : public GameObject {
public:
	Map() {
		terrainUtil.SetShader(pseudoShader);
		terrainUtil.SetMesh("pTerrain");
		terrainUtil.Scale(XMFLOAT3(5.0, 5.0, 5.0));
		terrainUtil.SetColor(XMFLOAT3(0.133333, 0.545098, 0.133333));
	}

	void Render(CommandList CmdList) {
		terrainUtil.Render(CmdList);
	}
};