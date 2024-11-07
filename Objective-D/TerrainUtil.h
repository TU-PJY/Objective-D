#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "textureUtil.h"

class TerrainUtil {
private:
	Mesh* TerrainMesh{};
	XMFLOAT4X4 TerrainMatrix = Mat4::Identity();

public:
	void InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData);
	bool CheckFloor(XMFLOAT3 Position, float HeightOffset = 0.0);
	void ClampToFloor(XMFLOAT3& Position, float HeightOffset = 0.0);
	float GetFloorHeight(float x, float z, float HeightOffset = 0.0);
	bool CheckCollision(XMFLOAT3& Position, float HeightOffset = 0.0);
};
extern TerrainUtil terrainUtil;
