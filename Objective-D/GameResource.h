#pragma once
#include "MeshUtil.h"
#include "ObjectShader.h"
#include "TextureUtil.h"

////////////////////////////////// 기본 리소스
extern BasicObjectShader* ObjectShader;
extern BasicObjectShader* BoundboxShader;

// 선 출력용 텍스처
extern Texture* LineTex;

// 이미지 출력용 매쉬
extern Mesh* ImagePannel;

// 스카이박스 출력용 매쉬
extern Mesh* SkyboxMesh;

// 바운드박스 출력용 매쉬
extern Mesh* BoundMesh;

// 바운드 스페어 출력용 매쉬
extern Mesh* BoundingSphereMesh;
//////////////////////////////////

// 커스텀 매쉬 및 텍스처
extern Mesh* GunMesh;
extern Texture* Tex, * WoodTex, * SkyboxTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);