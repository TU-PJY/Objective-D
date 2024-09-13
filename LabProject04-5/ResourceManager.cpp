#include "ResourceManager.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// DirectX_3D_Main::Init()에서 실행된다.

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