#include "ResourceManager.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Framework::Init()에서 실행된다.

BasicObjectShader* BasicShader;
Mesh* GunMesh;
Texture* Tex, *WoodTex;

void LoadShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	BasicShader = new BasicObjectShader();
	BasicShader->CreateShader(Device, RootSignature);
	BasicShader->CreateShaderDepthNone(Device, RootSignature);
}

void LoadMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	GunMesh = new Mesh(Device, CmdList, "Resources//Models//model.bin", MeshType::Binary);
}

void LoadTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	Tex = new Texture(Device, CmdList, L"Resources//Image//Gun.jpg");
	WoodTex = new Texture(Device, CmdList, L"Resources//Image//Wood.jpg");
}