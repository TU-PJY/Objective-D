#pragma once
#include "Object.h"
#include <random>

class Aircraft : public OBJ {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};
	float SpeedForward{};
	float SpeedStrafe{};

public:
	Aircraft(Layer layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		ObjectLayer = layer;
		Tag = tag;
	}

	void MoveAircraft(float FT) {
		if (MoveFront)
			LerpAcc(SpeedForward, 0.05, 5, FT);
		if (MoveBack)
			LerpAcc(SpeedForward, -0.05, 5, FT);
		if (MoveRight)
			LerpAcc(SpeedStrafe, 0.05, 5, FT);
		if (MoveLeft)
			LerpAcc(SpeedStrafe, -0.05, 5, FT);

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

		for (int i = 0; i < fw.Size(Layer::L2); ++i) {
			auto obj = fw.FindObject("test_obj", Layer::L2, i);
			if (fw.CheckCollision(this, obj))
				fw.DeleteObject(obj, Layer::L2);
		}

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


	void ObjectMouseMotionController(POINT CursorPos, POINT PrevCursorPos, bool LButtonDownState, bool RButtonDownState) {
		if (LButtonDownState) {
			float cxDelta = 0.0;
			float cyDelta = 0.0;

			cxDelta = (float)(CursorPos.x - PrevCursorPos.x) / 3.0f;
			cyDelta = (float)(CursorPos.y - PrevCursorPos.y) / 3.0f;
			::SetCursorPos(PrevCursorPos.x, PrevCursorPos.y);

			UpdateRotation(cyDelta, cxDelta, 0.0); 
		}
	}
};






class TestObject : public OBJ {
private:

public:
	TestObject(Layer layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(1.0, 0.0, 0.0));

		std::random_device rd;
		std::uniform_real_distribution urd{ -30.0, 30.0 };

		Position.x = urd(rd);
		Position.y = urd(rd) / 2;
		Position.z = 30;

		ObjectLayer = layer;
		Tag = tag;

		SetPosition(Position);
		UpdateOOBB();
	}
};