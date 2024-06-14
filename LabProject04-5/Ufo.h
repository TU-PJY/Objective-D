#pragma once
#include "Object.h"
#include <random>

class Ufo : public OBJ {
private:
	float Rotation{};
	bool MoveFront{}, MoveBack{}, MoveRight{}, MoveLeft{};

public:
	Ufo(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pUfoMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;
	}


	void MoveUfo(float FT) {
		if (MoveFront) {
			MoveForward(FT * 10);

		}
		if (MoveBack) {
			MoveForward(-FT * 10);
		}
		if (MoveRight)
			Rotation += 50 * FT;
		if (MoveLeft)
			Rotation -= 50 * FT;
	}


	void Update(float FT) {
		InitTransform();

		MoveUfo(FT);

		SetPosition(Position);
		Rotate(0.0, Rotation, 0.0);

		cam.SetPosition(Vec3::Add(Position, cam.GetOffset()));
		cam.TrackObject(Position, this, FT);
		cam.RegenerateViewMatrix();
	}


	void ObjectController(UINT nMessageID, WPARAM wParam) {
		switch (nMessageID) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_UP:
				MoveFront = true;
				break;

			case VK_DOWN:
				MoveBack = true;
				break;

			case VK_RIGHT:
				MoveRight = true;
				break;

			case VK_LEFT:
				MoveLeft = true;
				break;
			}
			break;

		case WM_KEYUP:
			switch (wParam) {
			case VK_UP:
				MoveFront = false;
				break;

			case VK_DOWN:
				MoveBack = false;
				break;

			case VK_RIGHT:
				MoveRight = false;
				break;

			case VK_LEFT:
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