#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"

class OOBB {
private:
	BoundingOrientedBox oobb = BoundingOrientedBox();
	bool Collide{};

	XMFLOAT4X4 TranslateMatrix{};
	XMFLOAT4X4 RotateMatrix{};
	XMFLOAT4X4 ScaleMatrix{};

	XMFLOAT3 BoundboxColor{ 1.0, 0.0, 0.0 };

public:
	void Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix, XMFLOAT4X4& SMatrix);
	void Render(ID3D12GraphicsCommandList* CmdList);
	bool CheckCollision(const OOBB& Other);
	bool CheckCollision(const BoundingBox& Other);
	bool CheckCollision(const BoundingSphere& Other);
};


class AABB {
private:
	BoundingBox aabb = BoundingBox();
	bool Collide{};

	XMFLOAT4X4 TranslateMatrix{};
	XMFLOAT4X4 ScaleMatrix{};

	XMFLOAT3 BoundboxColor{ 1.0, 0.0, 0.0 };

public:
	void Update(XMFLOAT3 Position, XMFLOAT3 Size);
	void Render(ID3D12GraphicsCommandList* CmdList);
	bool CheckCollision(const AABB& Other);
	bool CheckCollision(const BoundingSphere& Other);
	bool CheckCollision(const BoundingOrientedBox& Other);
};


class Range {
private:
	BoundingSphere sphere = BoundingSphere();
	bool Collide{};

	XMFLOAT4X4 TranslateMatrix{};
	XMFLOAT4X4 ScaleMatrix{};

	XMFLOAT3 BoundboxColor{ 1.0, 0.0, 0.0 };

public:
	void Update(const XMFLOAT3& Center, float Size);
	bool CheckCollision(const Range& Other);
	bool CheckCollision(const BoundingBox& Other);
	bool CheckCollision(const BoundingOrientedBox& Other);
	void Render(ID3D12GraphicsCommandList* CmdList);
};