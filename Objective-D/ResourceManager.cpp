#include "ResourceManager.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Framework::Init()에서 실행된다.

BasicObjectShader* BasicShader;
Texture* LineTex;

Mesh* GunMesh;
Texture* Tex, * WoodTex, * SkyboxTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	BasicShader = new BasicObjectShader();
	BasicShader->CreateShader(Device, RootSignature);
	BasicShader->CreateShaderDepthNone(Device, RootSignature);
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	// 이미지 출력용 패널 생성
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	GunMesh = new Mesh(Device, CmdList, "Resources//Models//model.bin", MeshType::Binary);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	// 선 그리기용 텍스처 생성
	LineTex = new Texture(Device, CmdList, L"Resources//Image//line_tex.png");

	Tex = new Texture(Device, CmdList, L"Resources//Image//Gun.jpg");
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg");
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//skytex.png");
}