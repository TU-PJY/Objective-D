#pragma once
#include "DirectX_3D.h"
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "textureUtil.h"

class TerrainUtil {
private:
	Mesh* TerrainMesh{};
	Shader* TerrainShader{};
	Texture* TerrainTexture{};

	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT3 TerrainColor{};

public:
	void SetMesh(Mesh* MeshPtr);
	void SetShader(Shader* ShaderPtr);
	void SetTexture(Texture* TexturePtr);
	void SetColor(XMFLOAT3 ColorValue);
	bool CheckFloor(XMFLOAT3 Position);
	void ClampToFloor(XMFLOAT3& Position);
	void CheckCollision(XMFLOAT3& Position);
	void Scale(XMFLOAT3 SizeValue);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void SendLightInfo(ID3D12GraphicsCommandList* CmdList);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, HeapAndBuffer& HAB_Struct, bool H_Flip, bool V_Flip, int BufferIndex);
	void SetAlpha(ID3D12GraphicsCommandList* CmdList, HeapAndBuffer& HAB_Struct, float AlphaValue, int BufferIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
};
extern TerrainUtil terrainUtil;
