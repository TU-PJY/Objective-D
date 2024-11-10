#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "textureUtil.h"

class TerrainUtil {
public:
	XMFLOAT4X4 TerrainMatrix = Mat4::Identity();
	Mesh* TerrainMesh{};

	void InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData);
	void ClampToFloor(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset = 0.0);
	void CheckCollision(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset=0.0);
	bool CheckFloor(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset = 0.0);
	float GetFloorHeight(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset = 0.0);
};
