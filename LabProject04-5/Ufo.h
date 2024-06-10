#pragma once
#include "D3D_Header.h"
#include "Scene.h"
#include "Object.h"

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