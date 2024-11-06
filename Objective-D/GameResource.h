#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

////////////////////////////////// �⺻ ���ҽ�
extern Object_Shader* ObjectShader;
extern Boundbox_Shader* BoundboxShader;
extern Image_Shader* ImageShader;
extern Line_Shader* LineShader;


// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

// �ٿ��ڽ� ��¿� �Ž�
extern Mesh* BoundMesh;

// �ٿ�� ����� ��¿� �Ž�
extern Mesh* BoundingSphereMesh;
//////////////////////////////////

// Ŀ���� �Ž� �� �ؽ�ó
extern Mesh* GunMesh;
extern Texture* Tex, * SkyboxTex, *WoodTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);