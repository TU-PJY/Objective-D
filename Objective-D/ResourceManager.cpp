#include "ResourceManager.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Framework::Init()���� ����ȴ�.

BasicObjectShader* BasicShader;
Mesh* GunMesh;
Texture* Tex, *WoodTex, *SkyboxTex;

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	BasicShader = new BasicObjectShader();
	BasicShader->CreateShader(Device, RootSignature);
	BasicShader->CreateShaderDepthNone(Device, RootSignature);
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	// �̹��� ��¿� �г� ����
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePanelMesh(Device, CmdList);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	GunMesh = new Mesh(Device, CmdList, "Resources//Models//model.bin", MeshType::Binary);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	Tex = new Texture(Device, CmdList, L"Resources//Image//Gun.jpg");
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg");
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//skytex.png");
}