#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

// 매쉬, 텍스처 로드 시 포인터를 저장하는 벡터
// 한꺼번에 업로드 버퍼를 삭제함
extern std::vector<Mesh*> LoadedMeshList;
extern std::vector<Texture*> LoadedTextureList;

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
void ClearUploadBuffer();