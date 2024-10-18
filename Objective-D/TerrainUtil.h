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

	float AlphaValue{ 1.0f };

public:
	void SetMesh(Mesh* MeshPtr);
	void SetShader(Shader* ShaderPtr);
	void SetTexture(Texture* TexturePtr);
	bool CheckFloor(XMFLOAT3 Position);
	void ClampToFloor(XMFLOAT3& Position);
	void CheckCollision(XMFLOAT3& Position);
	void SetColor(float R, float G, float B);
	void Scale(XMFLOAT3 SizeValue);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void InputLightInfo(ID3D12GraphicsCommandList* CmdList);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, bool H_Flip, bool V_Flip);
	void SetAlpha(ID3D12GraphicsCommandList* CmdList, float AlphaValue);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
};
extern TerrainUtil terrainUtil;
