#include "ResourceList.h"

Mesh* MeshPlayer;

void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	MeshPlayer = new Mesh(Device, CmdList, "Models//playermesh.txt", MeshType::Text);
}