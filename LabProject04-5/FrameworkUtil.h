#pragma once
#include "Config.h"
#include "ObjectBase.h"
#include "CameraUtil.h"
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include "ResourceList.h"
#include <deque>
#include <ranges>
#include <map>

// global scope shader
extern PseudoLightingShader* pShader;
constexpr int NUM_LAYER = static_cast<int>(Layer::END);

enum class ObjectRange
{ Single, All };

enum class LayerRange
{ Single, All };

typedef std::string(*Function)(void);

class Framework {
private:
	std::map<std::string, Mesh*> LoadedMeshList;
	std::map<std::string, Mesh*> LoadedTerrainList;
	std::multimap<std::string, OBJ*> ObjectList;

	std::string RunningMode{};
	Function ControllerPtr{};

protected:
	ID3D12RootSignature* RootSignature{};
	std::array<std::deque<OBJ*>, NUM_LAYER> Container;

public:
	POINT PrevCursorPosition{};
	bool LButtonDownState{}, RButtonDownState{};

	void Init(ID3D12Device *Device, ID3D12GraphicsCommandList *CmdList);

	void SwitchMode(Function ModeFunction) {
		ClearAll();
		RunningMode = ModeFunction();
	}

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotion(HWND hwnd);

	void Update(float FT) {
		for (int i = 0; i < NUM_LAYER; ++i) {
			for (auto It = std::begin(Container[i]); It != std::end(Container[i]); ++It) {
				if(!(*It)->DeleteDesc)
					(*It)->Update(FT);
			}

			UpdateContainer(i);
		}
	}

	void Render(ID3D12GraphicsCommandList* CmdList) {
		cam.SetViewportsAndScissorRects(CmdList);
		cam.UpdateShaderVariables(CmdList);

		for (int i = 0; i < NUM_LAYER; ++i) {
			for (auto It = std::begin(Container[i]); It != std::end(Container[i]); ++It) {
				if (!(*It)->DeleteDesc)
					(*It)->Render(CmdList);
			}
		}
	}

	void UpdateContainer(int Index) {
		std::erase_if(ObjectList, [](const std::pair<std::string, OBJ*>& Object) {
			return Object.second->DeleteDesc;
			});

		for (auto It = std::begin(Container[Index]); It != std::end(Container[Index]);) {
			if ((*It)->DeleteDesc) {
				delete* It;
				*It = nullptr;
				It = Container[Index].erase(It);
				continue;
			}
				
			++It;
		}
	}

	void AddObject(OBJ*&& Object, std::string Tag, Layer Layer) {
		int layer = static_cast<int>(Layer);

		Container[layer].push_back(Object);
		Container[layer].back()->ObjectTag = Tag;
		ObjectList.insert(std::pair(Tag, Container[layer].back()));
	}

	void DeleteSelf(OBJ* Object) {
		Object->DeleteDesc = true;
	}

	void DeleteObject(std::string Tag) {
		auto It = ObjectList.find(Tag);
		if (It != std::end(ObjectList))
			It->second->DeleteDesc = true;
	}

	void DeleteObject(std::string Tag, Layer TargetLayer) {
		int layer = static_cast<int>(TargetLayer);
		size_t NumObject = Container[layer].size();

		for (int i = 0; i < NumObject; ++i) {
			if (Container[layer][i]->ObjectTag == Tag)
				Container[layer][i]->DeleteDesc = true;
		}
	}

	OBJ* Find(std::string Tag) {
		auto It = ObjectList.find(Tag);
		if (It != std::end(ObjectList))
			return It->second;
		else
			return nullptr;
	}

	OBJ* Find(std::string Tag, Layer TargetLayer, int Index) {
		int layer = static_cast<int>(TargetLayer);

		if (Container[layer][Index]->ObjectTag == Tag)
			return Container[layer][Index];
		else
			return nullptr;
	}

	void ClearAll() {
		for (const auto& O : ObjectList)
			O.second->DeleteDesc = true;
	}

	bool CheckCollision(OBJ* From, OBJ* To) {
		if (From && To) {
			if (From->OOBB.Intersects(To->OOBB))
				return true;
			else 
				return false;
		}

		else
			return false;
	}

	bool CheckPickingByCursor(LPARAM lParam, OBJ* Object) {
		float xClient = LOWORD(lParam);
		float yClient = HIWORD(lParam);

		XMFLOAT3 xmf3PickPosition;
		xmf3PickPosition.x = (((2.0f * xClient) / (float)cam.CamViewport.Width) - 1) / cam.Cam4x4Projection._11;
		xmf3PickPosition.y = -(((2.0f * yClient) / (float)cam.CamViewport.Height) - 1) / cam.Cam4x4Projection._22;
		xmf3PickPosition.z = 1.0f;

		XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
		XMMATRIX xmmtxView = XMLoadFloat4x4(&cam.Cam4x4View);

		int nIntersected = 0;
		float fNearestHitDistance = FLT_MAX;
		float fHitDistance = FLT_MAX;
		nIntersected = Object->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);

		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance)) 
			return true;

		return false;
	}

	bool CheckPickingByCoordinate(float X, float Y, OBJ* Object) {
		float xClient = (X + 1.0) / 2.0 * WIDTH;
		float yClient = (1.0 - Y) / 2.0 * HEIGHT;

		XMFLOAT3 xmf3PickPosition;
		xmf3PickPosition.x = (((2.0f * xClient) / (float)cam.CamViewport.Width) - 1) / cam.Cam4x4Projection._11;
		xmf3PickPosition.y = -(((2.0f * yClient) / (float)cam.CamViewport.Height) - 1) / cam.Cam4x4Projection._22;
		xmf3PickPosition.z = 1.0f;

		XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
		XMMATRIX xmmtxView = XMLoadFloat4x4(&cam.Cam4x4View);

		int nIntersected = 0;
		float fNearestHitDistance = FLT_MAX;
		float fHitDistance = FLT_MAX;
		nIntersected = Object->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);

		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
			return true;

		return false;
	}

	bool CheckTerrainFloor(OBJ* Object, OBJ* Terrain) {
		if (Terrain->TerrainMesh) {
			if (Object->Position.y < Terrain->TerrainMesh->GetHeightAtPosition(Terrain->TerrainMesh, Object->Position.x, Object->Position.z, Terrain->Matrix))
				return true;
		}

		return false;
	}

	void MoveToTerrainFloor(OBJ* Object, OBJ* Terrain) {
		Object->Position.y = Terrain->TerrainMesh->GetHeightAtPosition(Terrain->TerrainMesh, Object->Position.x, Object->Position.z, Terrain->Matrix);
	}
	
	void CheckCollisionnTerrain(OBJ* Object, OBJ* Terrain) {
		if (Terrain->TerrainMesh) {
			if (Object->Position.y < Terrain->TerrainMesh->GetHeightAtPosition(Terrain->TerrainMesh, Object->Position.x, Object->Position.z, Terrain->Matrix))
				Object->Position.y = Terrain->TerrainMesh->GetHeightAtPosition(Terrain->TerrainMesh, Object->Position.x, Object->Position.z, Terrain->Matrix);
		}
	}

	void LoadMeshFromList(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
		for (const auto& [MeshName, Directory] : MeshList)
			LoadedMeshList.insert(std::pair(MeshName, MeshLoader(Device, CmdList, Directory)));
	}

	void LoadTerrainFromList(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
		for (const auto& [MeshName, Directory] : TerrainList)
			LoadedTerrainList.insert(std::pair(MeshName, MeshLoader(Device, CmdList, Directory)));
	}

	Mesh* FindMesh(std::string MeshName) {
		return LoadedMeshList.find(MeshName)->second;
	}

	Mesh* FindTerrain(std::string TerrainName) {
		return LoadedTerrainList.find(TerrainName)->second;
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
			for (auto It = std::ranges::begin(Container[i]); It != std::ranges::end(Container[i]); ++It)
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