#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"

class OOBB {
private:
	BoundingOrientedBox oobb = BoundingOrientedBox();
	bool Collide{};

public:
	void Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix);
	bool CheckCollision(const OOBB& Other);
};

