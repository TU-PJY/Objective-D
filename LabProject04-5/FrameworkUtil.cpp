#include "FrameworkUtil.h"

void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList,
	std::string ModeFunction(),
	void KeyboardControllerPtr(HWND, UINT, WPARAM, LPARAM),
	void MouseControllerPtr(HWND, UINT, WPARAM, LPARAM),
	void MouseMotionControllerPtr(HWND)) {
	RootSignature = CreateGraphicsRootSignature(Device);
	pseudoShader = framework.LoadShader(RootSignature, Device, CmdList);

	// add dummy object
	for (int i = 0; i < NUM_LAYER; ++i)
		Container[i].push_back(new DUMMY);

	SwitchMode(ModeFunction, KeyboardControllerPtr, MouseControllerPtr, MouseMotionControllerPtr);
}

std::string Framework::Mode() {
	return RunningMode;
}

void Framework::SwitchMode(std::string ModeFunction(),
	void KeyboardControllerPtr(HWND, UINT, WPARAM, LPARAM),
	void MouseControllerPtr(HWND, UINT, WPARAM, LPARAM),
	void MouseMotionControllerPtr(HWND)) {
	ClearAll();
	RunningMode = ModeFunction();
	KeyboardController = KeyboardControllerPtr;
	MouseController = MouseControllerPtr;
	MouseMotionController = MouseMotionControllerPtr;
}

void Framework::InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (KeyboardController)
		KeyboardController(hWnd, nMessageID, wParam, lParam);
}

void Framework::InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (MouseController)
		MouseController(hWnd, nMessageID, wParam, lParam);
}

void Framework::InputMouseMotion(HWND hWnd) {
	if (MouseMotionController)
		MouseMotionController(hWnd);
}

void Framework::Exit() {
	PostQuitMessage(1);
}

void Framework::Update(float FT) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}

		UpdateContainer(i);
	}
}

void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

void Framework::UpdateContainer(int Index) {
	std::erase_if(ObjectList, [](const std::pair<std::string, GameObject*>& Object) {
		return Object.second->DeleteMark;
		});

	for (auto It = std::begin(Container[Index]); It != std::end(Container[Index]);) {
		if ((*It)->DeleteMark) {
			delete* It;
			*It = nullptr;
			It = Container[Index].erase(It);
			continue;
		}

		++It;
	}
}

void Framework::AddObject(GameObject*&& Object, std::string Tag, Layer Layer) {
	int layer = static_cast<int>(Layer);

	Container[layer].push_back(Object);
	Container[layer].back()->ObjectTag = Tag;
	ObjectList.insert(std::pair(Tag, Container[layer].back()));
}

void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

void Framework::DeleteObject(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList))
		It->second->DeleteMark = true;
}

void Framework::DeleteObject(std::string Tag, Layer TargetLayer) {
	int layer = static_cast<int>(TargetLayer);
	size_t NumObject = Container[layer].size();

	for (int i = 0; i < NumObject; ++i) {
		if (Container[layer][i]->ObjectTag == Tag)
			Container[layer][i]->DeleteMark = true;
	}
}

GameObject* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList))
		return It->second;
	else
		return nullptr;
}

GameObject* Framework::Find(std::string Tag, Layer TargetLayer, int Index) {
	int layer = static_cast<int>(TargetLayer);

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];
	else
		return nullptr;
}

void Framework::ClearAll() {
	for (const auto& O : ObjectList)
		O.second->DeleteMark = true;
}

bool Framework::CheckCollision(const BoundingOrientedBox& OOBBFrom, const BoundingOrientedBox& OOBBTo) {
	if (OOBBFrom.Intersects(OOBBTo))
		return true;
	else
		return false;
}

bool Framework::CheckTerrainFloor(XMFLOAT3 Position, GameObject* Terrain) {
	if (Terrain->GetTerrainMesh()) {
		if (Position.y < Terrain->GetTerrainMesh()->GetHeightAtPosition(Terrain->GetTerrainMesh(), Position.x, Position.z, Terrain->GetTerrainMatrix()))
			return true;
	}

	return false;
}

void Framework::ClampToTerrainFloor(XMFLOAT3& Position, GameObject* Terrain) {
	Position.y = Terrain->GetTerrainMesh()->GetHeightAtPosition(Terrain->GetTerrainMesh(), Position.x, Position.z, Terrain->GetTerrainMatrix());
}

void Framework::CheckCollisionTerrain(XMFLOAT3& Position, GameObject* Terrain) {
	if (Terrain->GetTerrainMesh()) {
		if (Position.y < Terrain->GetTerrainMesh()->GetHeightAtPosition(Terrain->GetTerrainMesh(), Position.x, Position.z, Terrain->GetTerrainMatrix()))
			Position.y = Terrain->GetTerrainMesh()->GetHeightAtPosition(Terrain->GetTerrainMesh(), Position.x, Position.z, Terrain->GetTerrainMatrix());
	}
}

PseudoLightingShader* Framework::LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	PseudoLightingShader* shader = new PseudoLightingShader();
	shader->CreateShader(Device, RootSignature);
	shader->CreateShaderVariables(Device, CmdList);

	return shader;
}

ID3D12RootSignature* Framework::CreateGraphicsRootSignature(ID3D12Device* Device) {
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

ID3D12RootSignature* Framework::GetGraphicsRootSignature() {
	return(RootSignature);
}

void Framework::ReleaseObjects() {
	if (RootSignature)
		RootSignature->Release();
}

void Framework::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	CmdList->SetGraphicsRootSignature(RootSignature);
}