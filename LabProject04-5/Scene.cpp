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

std::string Mode1(){
	scene.AddObject(new Ufo(D3D_Layer::L1, "ufo"), D3D_Layer::L1);

	return __func__;
}


void Scene::InitScene(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	pShader = ShaderLoader(m_pd3dGraphicsRootSignature, pd3dDevice, pd3dCommandList);

	pUfoMesh = MeshLoader(pd3dDevice, pd3dCommandList, "Models//UFO.txt");
	pFlyerMesh = MeshLoader(pd3dDevice, pd3dCommandList, "Models//FlyerPlayership.txt");

	RunningMode = Mode1();
}