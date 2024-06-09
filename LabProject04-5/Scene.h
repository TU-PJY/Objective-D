#pragma once
#include "D3D_CONF.h"
#include "GlobalRes.h"
#include "Ufo.h"
#include <array>
#include <deque>
#include <ranges>


constexpr int NUM_MAIN_LAYER = static_cast<int>(MainLayer::END);


class Scene {
protected:
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;

	std::array<std::deque<MAIN_OBJ*>, NUM_MAIN_LAYER> MainCont;


public:
	void InitScene(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);

	void Update(float fTimeElapsed) {
		for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
			for (auto It = std::ranges::begin(MainCont[i]); It != std::ranges::end(MainCont[i]); ) {
				if (*It) (*It)->Update(fTimeElapsed);
				if (*It) ++It;
			}
		}
	}

	void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = NULL) {
		pCamera->SetViewportsAndScissorRects(pd3dCommandList);
		pCamera->UpdateShaderVariables(pd3dCommandList);

		for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
			for (auto It = std::ranges::begin(MainCont[i]); It != std::ranges::end(MainCont[i]); ) {
				if (*It) (*It)->Render(pd3dCommandList, pCamera);
				if (*It) ++It;
			}
		}
	}

	void AddObject(MAIN_OBJ* object, MainLayer mainlayer) {
		int layer = static_cast<int>(mainlayer);

		MainCont[layer].push_back(object);
	}


	void DeleteObject(MAIN_OBJ* object, MainLayer mainlayer) {
		int layer = static_cast<int>(mainlayer);

		auto It = std::ranges::find(MainCont[layer], object);
		if (It != std::ranges::end(MainCont[layer])) {
			delete* It;
			*It = nullptr;
			It = MainCont[layer].erase(It);
		}
	}


	Mesh* MeshLoader(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* Directory, bool TextMode = true) {
		Mesh* mesh = new Mesh(pd3dDevice, pd3dCommandList, Directory, TextMode);

		return mesh;
	}


	PseudoLightingShader* ShaderLoader(ID3D12RootSignature* m_pd3dGraphicsRootSignature, ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {
		PseudoLightingShader* shader = new PseudoLightingShader();
		shader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
		shader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

		return shader;
	}


	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice) {
		ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;

		D3D12_ROOT_PARAMETER pd3dRootParameters[3];
		pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		pd3dRootParameters[0].Constants.Num32BitValues = 4; //Time, ElapsedTime, xCursor, yCursor
		pd3dRootParameters[0].Constants.ShaderRegister = 0; //Time
		pd3dRootParameters[0].Constants.RegisterSpace = 0;
		pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		pd3dRootParameters[1].Constants.Num32BitValues = 19; //16 + 3
		pd3dRootParameters[1].Constants.ShaderRegister = 1; //World
		pd3dRootParameters[1].Constants.RegisterSpace = 0;
		pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		pd3dRootParameters[2].Constants.Num32BitValues = 35; //16 + 16 + 3
		pd3dRootParameters[2].Constants.ShaderRegister = 2; //Camera
		pd3dRootParameters[2].Constants.RegisterSpace = 0;
		pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
		::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
		d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
		d3dRootSignatureDesc.pParameters = pd3dRootParameters;
		d3dRootSignatureDesc.NumStaticSamplers = 0;
		d3dRootSignatureDesc.pStaticSamplers = NULL;
		d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

		ID3DBlob* pd3dSignatureBlob = NULL;
		ID3DBlob* pd3dErrorBlob = NULL;
		D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
		pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);
		if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
		if (pd3dErrorBlob) pd3dErrorBlob->Release();

		return(pd3dGraphicsRootSignature);
	}


	ID3D12RootSignature* GetGraphicsRootSignature() { 
		return(m_pd3dGraphicsRootSignature); 
	}


	void Scene::ReleaseObjects() {
		if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	}


	void Scene::ReleaseUploadBuffers() {
		for (int i = 0; i < NUM_MAIN_LAYER; ++i) {
			for (auto It = std::ranges::begin(MainCont[i]); It != std::ranges::end(MainCont[i]); ++It) {
				if(*It) (*It)->ReleaseUploadBuffers();
			}
		 }
	}


	bool Scene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		return(false);
	}


	bool Scene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
		return(false);
	}


	bool Scene::ProcessInput() {
		return(false);
	}


	void Scene::PrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) {
		pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	}


	Scene() {};
	~Scene() {};
};
