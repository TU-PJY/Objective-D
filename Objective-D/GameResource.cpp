#include "GameResource.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Scene::Init()에서 실행된다.

////////////////////////////////
BasicObjectShader* BasicShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

Mesh* GunMesh;
Texture* Tex, * WoodTex, * SkyboxTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 파이프라인 생성이 곧 쉐이더 설정의 마무리이다.
	// 일반 렌더링 쉐이더 생성
	BasicShader = new BasicObjectShader();
	BasicShader->CreateDefaultPipeline(Device, RootSignature);

	// 깊이 검사 미포함 파이프라인 생성
	BasicShader->CreateImageDepthPipelineState(Device, RootSignature);

	// 바운드박스 쉐이더 생성
	BoundboxShader = new BasicObjectShader();
	BoundboxShader->CreateBoundboxPipeline(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 이미지 출력용 패널 생성
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// 바운드박스 출력용 매쉬 생성
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);

	// 바운드스페어 출력용 매쉬 생성
	BoundingSphereMesh = new Mesh(Device, CmdList, "Resources//Models//BoundingSphereMesh.txt", MeshType::Text);
	////////////////////////////////

	GunMesh = new Mesh(Device, CmdList, "Resources//Models//model.bin", MeshType::Binary);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 선 그리기용 텍스처 생성
	LineTex = new Texture(Device, CmdList, L"Resources//Image//line_tex.png");
	////////////////////////////////

	Tex = new Texture(Device, CmdList, L"Resources//Image//Gun.jpg");
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg");
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//skytex.png");
}