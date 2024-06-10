#include "D3D_Header.h"
#include "Scene.h"
#include "Ufo.h"

// global scope mesh
Mesh* pUfoMesh;
Mesh* pFlyerMesh;

// global scope shader
PseudoLightingShader* pShader;

// global scope scene
Scene scene;


void Scene::InitScene(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	pShader = ShaderLoader(m_pd3dGraphicsRootSignature, pd3dDevice, pd3dCommandList);

	pUfoMesh = MeshLoader(pd3dDevice, pd3dCommandList, "Models//UFO.txt");

	AddObject(new Ufo(MainLayer::L1, "ufo"), MainLayer::L1);
}