#include "GameResource.h"
#include "ResourceFileLink.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Scene::Init()에서 실행된다.

// ResourceFileLink.h에 작성한 파일 경로를 사용할 수 있다.

std::vector<Mesh*> LoadedMeshList;
std::vector<Texture*> LoadedTextureList;

////////////////////////////////
Object_Shader* ObjectShader;
Boundbox_Shader* BoundboxShader;
Image_Shader* ImageShader;
Line_Shader* LineShader;

Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

Mesh* GunMesh;
Texture* Tex, * SkyboxTex, *WoodTex;

void ReleaseUploadBuffer(auto Resource);

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 파이프라인 생성이 곧 쉐이더 설정의 마무리이다.
	// 일반 렌더링 쉐이더 생성
	ObjectShader = new Object_Shader();
	// 기본 파이프라인 생성
	ObjectShader->CreateDefaultPS(Device, RootSignature);
	// 깊이 검사 미포함 파이프라인 생성
	ObjectShader->CreateNoneDepthPS(Device, RootSignature);

	// 이미지 출력용 파이프라인 생성
	ImageShader = new Image_Shader();
	ImageShader->CreateNoneDepthPS(Device, RootSignature);

	// 바운드박스 쉐이더 생성
	BoundboxShader = new Boundbox_Shader();
	BoundboxShader->CreateWireframePS(Device, RootSignature);
	BoundboxShader->CreateDefaultPS(Device, RootSignature);

	// 라인 브러쉬 출력용 쉐이더 생성
	LineShader = new Line_Shader();
	LineShader->CreateNoneDepthPS(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 이미지 출력용 패널 생성
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);
	ReleaseUploadBuffer(ImagePannel);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);
	ReleaseUploadBuffer(SkyboxMesh);

	// 바운드박스 출력용 매쉬 생성
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	ReleaseUploadBuffer(BoundMesh);

	// 바운드스페어 출력용 매쉬 생성
	BoundingSphereMesh = new Mesh(Device, CmdList, BOUNDING_SPHERE_MESH_DIRECTORY, MESH_TYPE_TEXT);
	ReleaseUploadBuffer(BoundingSphereMesh);
	////////////////////////////////

	GunMesh = new Mesh(Device, CmdList, GUN_MESH_DIRECTORY, MESH_TYPE_BIN);
	ReleaseUploadBuffer(GunMesh);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	Tex = new Texture(Device, CmdList, GUN_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	ReleaseUploadBuffer(Tex);

	SkyboxTex = new Texture(Device, CmdList, SKY_BOX_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	ReleaseUploadBuffer(SkyboxTex);

	WoodTex = new Texture(Device, CmdList, WOOD_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	ReleaseUploadBuffer(WoodTex);
}



////////////////////////////////////////////////////////


// 업로드 버퍼를 가지는 객체들을 벡터에 담아 한꺼번에 처리한다.
// 매쉬, 텍스처 구분 없이 그냥 파라미터에 전달하면 된다.
void ReleaseUploadBuffer(auto Resource) {
	if constexpr (std::is_same_v<decltype(Resource), Mesh*>) 
		LoadedMeshList.emplace_back(Resource);

	else if constexpr (std::is_same_v<decltype(Resource), Texture*>) 
		LoadedTextureList.emplace_back(Resource);
}

// 업로드 버퍼를 삭제하고, 벡터를 비운다.
void ClearUploadBuffer() {
	for (auto const& MeshPtr : LoadedMeshList)
		MeshPtr->ReleaseUploadBuffers();

	for (auto const& TexturePtr : LoadedTextureList)
		TexturePtr->ReleaseUploadBuffers();

	LoadedMeshList.clear();
	LoadedTextureList.clear();
}