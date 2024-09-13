#include "ResourceManager.h"

Mesh* MeshPlayer;
PseudoLightingShader* pseudoShader;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	pseudoShader = new PseudoLightingShader(RootSignature, Device, CmdList);
}

void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	MeshPlayer = new Mesh(Device, CmdList, "Models//playermesh.txt", MeshType::Text);
}