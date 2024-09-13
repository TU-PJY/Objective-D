#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"

extern PseudoLightingShader* LightingShader;
extern Mesh* MeshPlayer;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);