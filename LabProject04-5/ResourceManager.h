#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

extern BasicObjectShader* BasicShader;
extern Mesh* GunMesh;
extern Texture* Tex, * WoodTex;;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);