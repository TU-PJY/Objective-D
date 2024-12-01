#pragma once
#include "MeshUtil.h"
#include "TextureUtil.h"
#include "ImageShader.h"
#include "ObjectShader.h"
#include "BoundboxShader.h"
#include "LineShader.h"

// 매쉬, 텍스처, 쉐이더 전역 뱐수를 여기에 extern 선언
extern Mesh* GunMesh;
/////////////////////////////////////////////////////////////////////////////////

extern Texture* Tex, * SkyboxTex, * WoodTex;
/////////////////////////////////////////////////////////////////////////////////

extern Object_Shader* ObjectShader;
extern Boundbox_Shader* BoundboxShader;
extern Image_Shader* ImageShader;
extern Line_Shader* LineShader;
/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////// 기본 리소스
// 매쉬, 텍스처 로드 시 포인터를 저장하는 벡터
// 한꺼번에 업로드 버퍼를 삭제함
extern std::vector<Mesh*> LoadedMeshList;
extern std::vector<Texture*> LoadedTextureList;

extern Mesh* ImagePannel;
extern Mesh* SkyboxMesh;
extern Mesh* BoundMesh;
extern Mesh* BoundingSphereMesh;

void LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadSystemMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void LoadTexture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void ClearUploadBuffer();

inline void ImportMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Mesh*& MeshPtr, char* Directory, int Type) {
	MeshPtr = new Mesh(Device, CmdList, Directory, Type);
	LoadedMeshList.emplace_back(MeshPtr);
}

inline void ImportTexture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Texture*& TexturePtr, wchar_t* Directory, int Type) {
	TexturePtr = new Texture(Device, CmdList, Directory, Type);
	LoadedTextureList.emplace_back(TexturePtr);
}