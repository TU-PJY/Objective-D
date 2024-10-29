#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

////////////////////////////////// �⺻ ���ҽ�
extern BasicObjectShader* BasicShader;
extern BasicObjectShader* BoundboxShader;

// �� ��¿� �ؽ�ó
extern Texture* LineTex;

// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

// �ٿ��ڽ� ��¿� �Ž�
extern Mesh* BoundMesh;
//////////////////////////////////

// Ŀ���� �Ž� �� �ؽ�ó
extern Mesh* GunMesh;
extern Texture* Tex, * WoodTex, * SkyboxTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);