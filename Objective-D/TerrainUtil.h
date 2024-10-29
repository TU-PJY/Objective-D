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
	bool CheckFloor(XMFLOAT3 Position);
	void ClampToFloor(XMFLOAT3& Position, float Offset);
	float GetFloorHeight(float x, float z, float Offset);
	void CheckCollision(XMFLOAT3& Position);
	void InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData);
};
extern TerrainUtil terrainUtil;
