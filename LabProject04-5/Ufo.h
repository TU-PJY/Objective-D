#pragma once
#include "D3D_Header.h"
#include "Scene.h"
#include "Object.h"

class Ufo : public MAIN_OBJ {
private:
	XMFLOAT3 position{ 5.0, 0.0, 5.0 };
	float rotation{};

public:
	Ufo(MainLayer mainlayer, std::string tag) {
		SetShader(pShader);
		SetMesh(pUfoMesh);
		SetColor(XMFLOAT3(0.8, 0.8, 0.8));

		Layer = mainlayer;
		Tag = tag;
	}

	void Update(float FT) {
		InitTransform();
		
		rotation += FT * 200;
		SetPosition(position);
		Rotate(0.0, rotation, 0.0);

		//if(rotation > 600)
			//scene.DeleteObject(this, Layer);
	}
};