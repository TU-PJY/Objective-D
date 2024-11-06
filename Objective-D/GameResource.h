#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

////////////////////////////////// 기본 리소스
extern Object_Shader* ObjectShader;
extern Boundbox_Shader* BoundboxShader;
extern Image_Shader* ImageShader;
extern Line_Shader* LineShader;


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
extern Texture* Tex, * SkyboxTex, *WoodTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);