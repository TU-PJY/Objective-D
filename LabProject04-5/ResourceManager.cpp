#include "ResourceManager.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// DirectX_3D_Main::Init()���� ����ȴ�.

Mesh* MeshPlayer;
PseudoLightingShader* LightingShader;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	LightingShader = new PseudoLightingShader();
	LightingShader->CreateShader(Device, RootSignature);
	LightingShader->CreateShaderVariables(Device, CmdList);
}

void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	MeshPlayer = new Mesh(Device, CmdList, "Models//UFO.txt", MeshType::Text);
}