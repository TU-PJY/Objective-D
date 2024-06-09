#pragma once
#include "stdafx.h"
#include "Object.h"
#include "GlobalRes.h"

class Ufo : public MAIN_OBJ {
private:
	XMFLOAT3 position{ 5.0, 0.0, 5.0 };
	float rotation{};

public:
	Ufo() {
		SetShader(pShader);
		SetMesh(pUfoMesh);
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));
	}

	void Update(float FT) {
		InitTransform();

		rotation += FT * 200;
		SetPosition(position);
		Rotate(0.0, rotation, 0.0);
	}
};