#pragma once
#include "ObjectBase.h"
#include <random>

class Aircraft : public BASE {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};
	float SpeedForward{};
	float SpeedStrafe{};

public:
	Aircraft() {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
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

		if(!MoveFront && !MoveBack)
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

		auto ptr = fw.Find("map");
		if (ptr) fw.CheckCollisionnTerrain(this, ptr);

		UpdateOOBB();
	}

	void ObjectKeyboardController(UINT nMessageID, WPARAM wParam) {
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


	void ObjectMouseMotionController(POINT PrevCursorPos, bool LButtonDownState, bool RButtonDownState) {
		if (LButtonDownState) {
			::SetCursor(NULL);
			POINT CursorPos;
			::GetCursorPos(&CursorPos);

			float cxDelta = 0.0;
			float cyDelta = 0.0;

			cxDelta = (float)(CursorPos.x - PrevCursorPos.x) / 5.0f;
			cyDelta = (float)(CursorPos.y - PrevCursorPos.y) / 5.0f;
			::SetCursorPos(PrevCursorPos.x, PrevCursorPos.y);

			UpdateRotation(cyDelta, cxDelta, 0.0); 
		}
	}
};


class TestObject : public BASE {
private:

public:
	TestObject(Layer layer, std::string tag, XMFLOAT3 position, XMFLOAT3 color) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		ModelColor = color;
		Position = position;

		UpdateOOBB();
	}

	void Update(float FT) {
		InitTransform();


		auto ptr = fw.Find("obj1");
		if (ptr) LookAt(ptr->Position, XMFLOAT3(0.0, 1.0, 0.0));

		Scale(0.5, 0.5, 0.5);
		SetPosition(Position);
	}
};


class Map : public BASE {
private:

public:
	Map() {
		SetShader(pShader);
		SetTerrain(fw.FindTerrain("pTerrain"));
		SetColor(XMFLOAT3(0.133333,	0.545098, 0.133333));
		Scale(5.0, 5.0, 5.0);
	}

	void ObjectKeyboardController(UINT nMessageID, WPARAM wParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			break;

		case WM_KEYUP:
			break;
		}
	}

	void ObjectMouseMotionController(POINT CursorPos, POINT PrevCursorPos, bool LButtonDownState, bool RButtonDownState) {}

	void ObjectMouseController(POINT CursorPos, bool LButtonDownState, bool RButtonDownState) {}
};