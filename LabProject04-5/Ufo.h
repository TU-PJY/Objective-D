#pragma once
#include "D3D_Header.h"
#include "Scene.h"
#include "Object.h"
#include <random>

class Ufo : public OBJ {
private:
	XMFLOAT3 Position{0.0, 0.0, 10.0};
	float Rotation{};
	bool rotate{};

public:
	Ufo(D3D_Layer layer, std::string tag) {
		SetShader(pShader);
		SetMesh(pUfoMesh);
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;

		std::random_device rd;
		std::uniform_real_distribution urd{ -10.0, 10.0 };

		Position.x = urd(rd);
		Position.y = urd(rd);
		Position.z = urd(rd);
	}


	void SetRotation() {
		if (!rotate)
			rotate = true;
		else
			rotate = false;
	}


	void Update(float FT) {
		InitTransform();
		
		if(rotate)
			Rotation += FT * 200;

		SetPosition(Position);
		Rotate(0.0, Rotation, 0.0);
	}
};


class Aircraft : public OBJ {
private:
	XMFLOAT3 Position{ 0.0, 0.0, 10.0 };
	float Rotation{};
	bool rotate{};

public:
	Aircraft(D3D_Layer layer, std::string tag) {
		SetShader(pShader);
		SetMesh(pFlyerMesh);
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;

		std::random_device rd;
		std::uniform_real_distribution urd{ -10.0, 10.0 };

		Position.x = urd(rd);
		Position.y = urd(rd);
		Position.z = urd(rd);
	}


	void SetRotation() {
		if (!rotate)
			rotate = true;
		else
			rotate = false;
	}


	void Update(float FT) {
		InitTransform();

		if (rotate)
			Rotation += FT * 200;

		SetPosition(Position);
		Rotate(0.0, Rotation, 0.0);
	}
};