#include "GameResource.h"
#include "ResourceFileLink.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.

// ResourceFileLink.h�� �ۼ��� ���� ��θ� ����� �� �ִ�.

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
	// ���������� ������ �� ���̴� ������ �������̴�.
	// �Ϲ� ������ ���̴� ����
	ObjectShader = new Object_Shader();
	// �⺻ ���������� ����
	ObjectShader->CreateDefaultPS(Device, RootSignature);
	// ���� �˻� ������ ���������� ����
	ObjectShader->CreateNoneDepthPS(Device, RootSignature);

	// �̹��� ��¿� ���������� ����
	ImageShader = new Image_Shader();
	ImageShader->CreateNoneDepthPS(Device, RootSignature);

	// �ٿ��ڽ� ���̴� ����
	BoundboxShader = new Boundbox_Shader();
	BoundboxShader->CreateWireframePS(Device, RootSignature);
	BoundboxShader->CreateDefaultPS(Device, RootSignature);

	// ���� �귯�� ��¿� ���̴� ����
	LineShader = new Line_Shader();
	LineShader->CreateNoneDepthPS(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �̹��� ��¿� �г� ����
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);
	ReleaseUploadBuffer(ImagePannel);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);
	ReleaseUploadBuffer(SkyboxMesh);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	ReleaseUploadBuffer(BoundMesh);

	// �ٿ�彺��� ��¿� �Ž� ����
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


// ���ε� ���۸� ������ ��ü���� ���Ϳ� ��� �Ѳ����� ó���Ѵ�.
// �Ž�, �ؽ�ó ���� ���� �׳� �Ķ���Ϳ� �����ϸ� �ȴ�.
void ReleaseUploadBuffer(auto Resource) {
	if constexpr (std::is_same_v<decltype(Resource), Mesh*>) 
		LoadedMeshList.emplace_back(Resource);

	else if constexpr (std::is_same_v<decltype(Resource), Texture*>) 
		LoadedTextureList.emplace_back(Resource);
}

// ���ε� ���۸� �����ϰ�, ���͸� ����.
void ClearUploadBuffer() {
	for (auto const& MeshPtr : LoadedMeshList)
		MeshPtr->ReleaseUploadBuffers();

	for (auto const& TexturePtr : LoadedTextureList)
		TexturePtr->ReleaseUploadBuffers();

	LoadedMeshList.clear();
	LoadedTextureList.clear();
}