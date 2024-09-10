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
	//bool CheckCollision(const AABB& Other);
	//bool CheckCollision(const Range& Other);
};


class AABB {
private:
	BoundingBox aabb = BoundingBox();
	bool Collide{};

public:
	void Update(const XMFLOAT3& Center, const XMFLOAT3& Size);
	bool CheckCollision(const AABB& Other);
	//bool CheckCollision(const OOBB& Other);
	//bool CheckCollision(const Range& Other);
};

class Range {
private:
	BoundingSphere sphere = BoundingSphere();
	bool Collide{};

public:
	void Update(const XMFLOAT3& Center, const float& Size);
	bool CheckCollision(const Range& Other);
	//bool CheckCollision(const OOBB& Other);
	//bool CheckCollision(const AABB& Other);
};