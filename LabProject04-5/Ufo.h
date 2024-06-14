#pragma once
#include "Object.h"
#include <random>

class Ufo : public OBJ {
private:
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};

public:
	Ufo(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;
	}


	void MoveUfo(float FT) {
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

		MoveUfo(FT);

		SetPosition(Position);
		Rotate(Rotation.x, Rotation.y, Rotation.z);

		cam.TrackObject(Position, this, FT);
		cam.RegenerateViewMatrix();
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


class Aircraft : public OBJ {
private:
	XMFLOAT3 Position{ 0.0, 0.0, 10.0 };
	float Rotation{};

public:
	Aircraft(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;

		std::random_device rd;
		std::uniform_real_distribution urd{ -10.0, 10.0 };

		Position.x = urd(rd);
		Position.y = 0.0;
		Position.z = urd(rd);
	}

	void Update(float FT) {
		InitTransform();

		SetPosition(Position);
	}
};