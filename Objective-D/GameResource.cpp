#include "GameResource.h"
#include "ResourceFileLink.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.

// ResourceFileLink.h�� �ۼ��� ���� ��θ� ����� �� �ִ�.

////////////////////////////////
Object_Shader* ObjectShader;
Boundbox_Shader* BoundboxShader;
Image_Shader* ImageShader;
Line_Shader* LineShader;

Texture* ColorTex;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

Mesh* GunMesh;
Texture* Tex, * WoodTex, * SkyboxTex;

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

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);

	// �ٿ�彺��� ��¿� �Ž� ����
	BoundingSphereMesh = new Mesh(Device, CmdList, BOUNDING_SPHERE_MESH_DIRECTORY, MESH_TYPE_TEXT);
	////////////////////////////////

	GunMesh = new Mesh(Device, CmdList, GUN_MESH_DIRECTORY, MESH_TYPE_BIN);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �� �׸���� �ؽ�ó ����
	ColorTex = new Texture(Device, CmdList, LINE_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	////////////////////////////////

	Tex = new Texture(Device, CmdList, GUN_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	WoodTex = new Texture(Device, CmdList, WOOD_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	SkyboxTex = new Texture(Device, CmdList, SKY_BOX_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
}