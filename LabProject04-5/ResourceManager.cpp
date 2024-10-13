#include "ResourceManager.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// DirectX_3D_Main::Init()���� ����ȴ�.

Mesh* GunMesh;
PseudoLightingShader* LightingShader;
ID3D12DescriptorHeap* srvHeap, *sampleHeap;
Texture Tex;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	LightingShader = new PseudoLightingShader();
	LightingShader->CreateShader(Device, RootSignature);
	LightingShader->CreateShaderVariables(Device, CmdList);
}

void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	GunMesh = new Mesh(Device, CmdList, "Models//model.bin", MeshType::Binary);
}