#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"

class OOBB {
private:
	BoundingOrientedBox oobb = BoundingOrientedBox();
	bool Collide{};

public:
	void Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix, XMFLOAT4X4& SMatrix);
	void Update(const XMFLOAT3& Center, const XMFLOAT3& Size, const XMFLOAT3& Rotation);
	bool CheckCollision(const OOBB& Other);
};

