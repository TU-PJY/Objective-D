#include "stdafx.h"
#include "Scene.h"


void Scene::InitScene(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	pUfoMesh = MeshLoader(pd3dDevice, pd3dCommandList, "Models//UFO.txt");
	pShader = ShaderLoader(m_pd3dGraphicsRootSignature, pd3dDevice, pd3dCommandList);

	AddObject(new Ufo(), MainLayer::L1);
}


