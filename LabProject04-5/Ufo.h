#pragma once
#include "Object.h"
#include <random>

class Ufo : public OBJ {
private:
	XMFLOAT3 Position{0.0, 0.0, 10.0};
	float Rotation{};
	bool rotate{};

public:
	Ufo(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pUfoMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;

		std::random_device rd;
		std::uniform_real_distribution urd{ -10.0, 10.0 };

		Position.x = urd(rd);
		Position.y = urd(rd) / 10;
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
	Aircraft(LayerFW layer, std::string tag) {
		SetShader(pShader);
		SetMesh(fw.FindMesh("pFlyerMesh"));
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = layer;
		Tag = tag;

		std::random_device rd;
		std::uniform_real_distribution urd{ -10.0, 10.0 };

		Position.x = urd(rd);
		Position.y = urd(rd) / 10;
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