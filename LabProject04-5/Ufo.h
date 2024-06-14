#pragma once
#include "Object.h"
#include <random>

class Aircraft : public OBJ {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};

public:
	Aircraft(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;
	}


	void MoveAircraft(float FT) {
		if (MoveFront) 
			MoveForward(FT * 10);
		if (MoveBack) 
			MoveForward(-FT * 10);
		if (MoveRight)
			MoveStrafe(FT * 10);
		if (MoveLeft)
			MoveStrafe(-FT * 10);
	}


	void Update(float FT) {
		InitTransform();

		MoveAircraft(FT);

		SetPosition(Position);
		Rotate(Rotation.x, Rotation.y, Rotation.z);

		//cam.TrackCamera(Position, this, FT);
	}


	void ObjectController(UINT nMessageID, WPARAM wParam) {
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
};