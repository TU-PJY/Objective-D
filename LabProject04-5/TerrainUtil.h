#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"
#include "ShaderUtil.h"

class TerrainUtil {
private:
	Mesh* TerrainMesh{};
	Shader* TerrainShader{};
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT3 TerrainColor{};

public:
	void SetMesh(Mesh* MeshPtr);
	void SetShader(Shader* ShaderPtr);
	void SetColor(XMFLOAT3 ColorValue);
	bool CheckFloor(XMFLOAT3 Position);
	void ClampToFloor(XMFLOAT3& Position);
	void CheckCollision(XMFLOAT3& Position);
	void Scale(XMFLOAT3 SizeValue);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
};
extern TerrainUtil terrainUtil;
