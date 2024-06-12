#include "Framework.h"
#include "Ufo.h"

// global scope shader
PseudoLightingShader* pShader;

// global scope camera
Camera cam;

// global scope fw
Framework fw;

// mesh list to load
std::unordered_map<std::string, char*> MeshList {
	{ "pUfoMesh", "Models//UFO.txt" },
	{ "pFlyerMesh", "Models//FlyerPlayerShip.txt" },
};


std::string Mode1(){
	for(int i = 0; i < 10; ++i)
		fw.AddObject(new Ufo(D3D_Layer::L1, "ufo"), D3D_Layer::L1);

	return __func__;
}


void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	RootSignature = CreateGraphicsRootSignature(Device);
	pShader = ShaderLoader(RootSignature, Device, CmdList);
	LoadMeshFromList(Device, CmdList);

	SetMode(Mode1);
}