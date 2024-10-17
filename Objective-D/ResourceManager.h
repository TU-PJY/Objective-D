#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

extern BasicObjectShader* BasicShader;
extern Mesh* GunMesh;
extern Texture* Tex, * WoodTex, * SkyboxTex;

// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);