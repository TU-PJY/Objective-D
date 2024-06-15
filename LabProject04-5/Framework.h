#pragma once
#include "CONF.h"
#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include <deque>
#include <ranges>
#include <unordered_map>

// global scope shader
extern PseudoLightingShader* pShader;

extern std::unordered_map<std::string, char*> MeshList;

constexpr int NUM_LAYER = static_cast<int>(LayerFW::END);

enum class ObjectRange
{ Single, All };

enum class LayerRange
{ Single, All };


class Framework {
private:
	std::string RunningMode{};
	std::unordered_map<std::string, Mesh*> LoadedMeshList;
	bool LButtonDownState{}, RButtonDownState{};
	typedef std::string(*Function)(void);

protected:
	ID3D12RootSignature* RootSignature = nullptr;
	std::array<std::deque<OBJ*>, NUM_LAYER> ObjectCont;

public:
	void Init(ID3D12Device *Device, ID3D12GraphicsCommandList *CmdList);

	void SetMode(Function ModeFunction) {
		ClearAll();
		RunningMode = ModeFunction();
	}

	void KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void MouseMotionController(POINT PrevCursorPosition, HWND hwnd);

	void Update(float FT) {
		for (int i = 0; i < NUM_LAYER; ++i) {
			for (auto It = std::ranges::begin(ObjectCont[i]); It != std::ranges::end(ObjectCont[i]); ) {
				if (*It) (*It)->Update(FT);
				if (*It) ++It;
			}
		}
	}

	void Render(ID3D12GraphicsCommandList* CmdList) {
		cam.SetViewportsAndScissorRects(CmdList);
		cam.UpdateShaderVariables(CmdList);

		for (int i = 0; i < NUM_LAYER; ++i) {
			for (auto It = std::ranges::begin(ObjectCont[i]); It != std::ranges::end(ObjectCont[i]); ) {
				if (*It) (*It)->Render(CmdList);
				if (*It) ++It;
			}
		}
	}

	void AddObject(OBJ*&& Object, LayerFW Layer) {
		int layer = static_cast<int>(Layer);

		ObjectCont[layer].push_back(Object);
	}

	void DeleteObject(OBJ* Object, LayerFW Layer) {
		int layer = static_cast<int>(Layer);

		auto It = std::ranges::find(ObjectCont[layer], Object);
		if (It != std::ranges::end(ObjectCont[layer])) {
			delete* It;
			*It = nullptr;
			It = ObjectCont[layer].erase(It);
		}
	}

	void DeleteObject(std::string ObjectTag, ObjectRange Range1, LayerRange Range2, LayerFW Layer = static_cast<LayerFW>(0)) {
		int layer = static_cast<int>(Layer);

		if (Range1 == ObjectRange::Single) {
			if (Range2 == LayerRange::Single) {
				auto It = std::ranges::find_if(ObjectCont[layer], [&ObjectTag](OBJ*& obj) { return obj->Tag == ObjectTag; });

				if (It != std::ranges::end(ObjectCont[layer])) {
					delete* It;
					*It = nullptr;
					It = ObjectCont[layer].erase(It);
				}
			}

			else if (Range2 == LayerRange::All) {
				for (int i = 0; i < NUM_LAYER; ++i) {
					auto It = std::ranges::find_if(ObjectCont[i], [&ObjectTag](OBJ*& obj) { return obj->Tag == ObjectTag; });

					if (It != std::ranges::end(ObjectCont[i])) {
						delete *It;
						*It = nullptr;
						It = ObjectCont[layer].erase(It);

						return;
					}
				}
			}
		}

		else if (Range1 == ObjectRange::All) {
			if (Range2 == LayerRange::Single) {
				auto It = std::ranges::begin(ObjectCont[layer]);

				while (It != std::ranges::end(ObjectCont[layer])) {
					It = std::ranges::find_if(ObjectCont[layer], [&ObjectTag](OBJ*& Obj) { return Obj->Tag == ObjectTag; });

					if (It != std::ranges::end(ObjectCont[layer])) {
						delete* It;
						*It = nullptr;
						It = ObjectCont[layer].erase(It);
					}
				}
			}

			else if (Range2 == LayerRange::All) {
				for (int i = 0; i < NUM_LAYER; ++i) {
					auto It = std::ranges::begin(ObjectCont[i]);

					while (It != std::ranges::end(ObjectCont[i])) {
						It = std::ranges::find_if(ObjectCont[i], [&ObjectTag](OBJ*& Obj) { return Obj->Tag == ObjectTag; });

						if (It != std::ranges::end(ObjectCont[i])) {
							delete* It;
							*It = nullptr;
							It = ObjectCont[i].erase(It);
						}
					}
				}
			}
		}
	}

	OBJ* FindObject(std::string ObjectTag, LayerRange Range1, LayerFW Layer = static_cast<LayerFW>(0)) {
		int layer = static_cast<int>(Layer);

		if (Range1 == LayerRange::Single) {
			auto It = std::ranges::find_if(ObjectCont[layer], [&ObjectTag](OBJ*& obj) { return obj->Tag == ObjectTag; });
			if (It != std::ranges::end(ObjectCont[layer]))
				return *It;
			else
				return nullptr;
		}

		else if (Range1 == LayerRange::All) {
			for (int i = 0; i < NUM_LAYER; ++i) {
				auto It = std::ranges::begin(ObjectCont[i]);

				while (It != std::ranges::end(ObjectCont[i])) {
					It = std::ranges::find_if(ObjectCont[i], [&ObjectTag](OBJ*& Obj) { return Obj->Tag == ObjectTag; });

					if (It != std::ranges::end(ObjectCont[i]))
						return *It;

					++It;
				}
			}
			return nullptr;
		}
	}

	OBJ* FindObject(std::string ObjectTag, LayerFW Layer, int Index) {
		int layer = static_cast<int>(Layer);

		if (ObjectCont[layer][Index]->Tag == ObjectTag)
			return ObjectCont[layer][Index];
		else
			return false;
	}

	size_t Size(LayerFW Layer) {
		int layer = static_cast<int>(Layer);

		return ObjectCont[layer].size();
	}

	void ClearAll() {
		for (int i = 0; i < NUM_LAYER; ++i) {
			auto It = std::ranges::begin(ObjectCont[i]);

			while (It != std::ranges::end(ObjectCont[i])) {
				delete* It;
				*It = nullptr;
				It = ObjectCont[i].erase(It);
			}
		}
	}

	void LoadMeshFromList(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
		for (const auto& [MeshName, Directory] : MeshList)
			LoadedMeshList.insert(make_pair(MeshName, MeshLoader(Device, CmdList, Directory)));
	}

	Mesh* FindMesh(std::string MeshName) {
		return LoadedMeshList.find(MeshName)->second;
	}

	Mesh* MeshLoader(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode = true) {
		Mesh* mesh = new Mesh(Device, CmdList, Directory, TextMode);

		return mesh;
	}

	PseudoLightingShader* ShaderLoader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
		PseudoLightingShader* shader = new PseudoLightingShader();
		shader->CreateShader(Device, RootSignature);
		shader->CreateShaderVariables(Device, CmdList);

		return shader;
	}

	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device) {
		ID3D12RootSignature* GraphicsRootSignature = NULL;

		D3D12_ROOT_PARAMETER RootParameters[3];
		RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		RootParameters[0].Constants.Num32BitValues = 4; //Time, ElapsedTime, xCursor, yCursor
		RootParameters[0].Constants.ShaderRegister = 0; //Time
		RootParameters[0].Constants.RegisterSpace = 0;
		RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		RootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		RootParameters[1].Constants.Num32BitValues = 19; //16 + 3
		RootParameters[1].Constants.ShaderRegister = 1; //World
		RootParameters[1].Constants.RegisterSpace = 0;
		RootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		RootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		RootParameters[2].Constants.Num32BitValues = 35; //16 + 16 + 3
		RootParameters[2].Constants.ShaderRegister = 2; //Camera
		RootParameters[2].Constants.RegisterSpace = 0;
		RootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		D3D12_ROOT_SIGNATURE_FLAGS RootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
		::ZeroMemory(&RootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
		RootSignatureDesc.NumParameters = _countof(RootParameters);
		RootSignatureDesc.pParameters = RootParameters;
		RootSignatureDesc.NumStaticSamplers = 0;
		RootSignatureDesc.pStaticSamplers = NULL;
		RootSignatureDesc.Flags = RootSignatureFlags;

		ID3DBlob* SignatureBlob = NULL;
		ID3DBlob* ErrorBlob = NULL;
		D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SignatureBlob, &ErrorBlob);
		Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&GraphicsRootSignature);
		if (SignatureBlob) SignatureBlob->Release();
		if (ErrorBlob) ErrorBlob->Release();

		return(GraphicsRootSignature);
	}

	ID3D12RootSignature* GetGraphicsRootSignature() { 
		return(RootSignature); 
	}

	void ReleaseObjects() {
		if (RootSignature) 
			RootSignature->Release();
	}

	void ReleaseUploadBuffers() {
		for (int i = 0; i < NUM_LAYER; ++i) {
			for (auto It = std::ranges::begin(ObjectCont[i]); It != std::ranges::end(ObjectCont[i]); ++It)
				if(*It) (*It)->ReleaseUploadBuffers();
		 }
	}

	void PrepareRender(ID3D12GraphicsCommandList* CmdList) {
		CmdList->SetGraphicsRootSignature(RootSignature);
	}

	Framework() {};
	~Framework() {};
};

// global scope framework
extern Framework fw;