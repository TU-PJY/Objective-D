#include "FrameworkUtil.h"
#include "ResourceList.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, std::string ModeFunction()) {
	// �ʿ��� ���ϵ��� ��� �ε��Ѵ�
	RootSignature = CreateGraphicsRootSignature(Device);
	pseudoShader = framework.LoadShader(RootSignature, Device, CmdList);
	LoadMeshResource(Device, CmdList);

	// ������Ʈ�� ����ϴ� �����̳ʿ��� �߰��ǳ�, ��ü ����, ��ü �˻����� ����ϴ� ��Ƽ�ʿ��� �߰����� �ʴ´�.
	for (int i = 0; i < NUM_LAYER; ++i)
		Container[i].push_back(new DUMMY);

	SwitchMode(ModeFunction);
}

// ���� ���� ���� ��� �̸��� �����Ѵ�
std::string Framework::Mode() {
	return RunningMode;
}

// ��带 �����Ѵ�. ��� ���� �� ���� ��忡 �ִ� ��ü���� ��� �����ȴ�.
void Framework::SwitchMode(std::string ModeFunction()) {
	ClearAll();
	RunningMode = ModeFunction();
}

// ��Ʈ�ѷ� ���� �Լ��̴�. �� �Լ��� ���� �� ���� ����,
void Framework::SetKeyController(void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardController = KeyboardControllerPtr;
}

void Framework::SetMouseController(void (*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseController = MouseControllePtr;
}

void Framework::SetMouseMotionController(void (*MouseMotionControllerPtr)(HWND)) {
	MouseMotionController = MouseMotionControllerPtr;
}

// Ű���, ���콺, ���콺 �������� ��ü �����Ϳ� �����Ѵ�. �� �Լ��� ���� ���� �� ���� ����.
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

// ���α׷��� �����Ѵ�.
void Framework::Exit() {
	PostQuitMessage(1);
}

// ���� �����ϴ� ��� ��ü���� ������Ʈ�Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü�� ������Ʈ���� �ʴ´�
void Framework::Update(float FT) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}

		// ������Ʈ�� ��� ��ģ �� ������Ʈ ���̾��� ���̾�� ���� ��ũ�� ǥ�õ� ��ü�� ã�� �����Ѵ�.
		UpdateContainer(i);
	}
}

// ���� �����ϴ� ��� ��ü���� �������Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü���� ���������� �ʴ´�.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

// ���� ��ũ�� ǥ�õ� ��ü���� �����Ѵ�.
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

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
void Framework::AddObject(GameObject*&& Object, std::string Tag, Layer Layer) {
	int layer = static_cast<int>(Layer);

	Container[layer].push_back(Object);
	Container[layer].back()->ObjectTag = Tag;
	ObjectList.insert(std::pair(Tag, Container[layer].back()));
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�. 
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�. 
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateContainer()���� ������ �����ȴ�.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// �±׸� ����Ͽ� ��ü�� �����Ѵ�.
void Framework::DeleteObject(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList))
		It->second->DeleteMark = true;
}

// Ư�� ���̾�� Ư�� �±׸� �˻��Ͽ� �ش�Ǵ� ��ü�� �����Ѵ�.
void Framework::DeleteObject(std::string Tag, Layer TargetLayer) {
	int layer = static_cast<int>(TargetLayer);
	size_t NumObject = Container[layer].size();

	for (int i = 0; i < NumObject; ++i) {
		if (Container[layer][i]->ObjectTag == Tag)
			Container[layer][i]->DeleteMark = true;
	}
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// ���� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList))
		return It->second;
	else
		return nullptr;
}

// Ư�� ���̾ �����ϴ� �ټ��� ��ü���� �����͸� ��� �����ϴµ��� ����Ѵ�.
// for���� �Բ� ����Ͼ� �Ѵ�.
GameObject* Framework::Find(std::string Tag, Layer TargetLayer, int Index) {
	int layer = static_cast<int>(TargetLayer);

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];
	else
		return nullptr;
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�. ���� ��ü�� ObjectList�� ���� �����Ƿ� ���� ��ũ�� ǥ�õ��� �ʈf��.
void Framework::ClearAll() {
	for (const auto& O : ObjectList)
		O.second->DeleteMark = true;
}

// ���̴��� �ε��Ѵ�.
PseudoLightingShader* Framework::LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	PseudoLightingShader* shader = new PseudoLightingShader();
	shader->CreateShader(Device, RootSignature);
	shader->CreateShaderVariables(Device, CmdList);

	return shader;
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
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

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Framework::GetGraphicsRootSignature() {
	return(RootSignature);
}

// ��Ʈ�ñ״�ó�� �������Ѵ�
void Framework::ReleaseObjects() {
	if (RootSignature)
		RootSignature->Release();
}

// �������� �غ��Ѵ�
void Framework::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	CmdList->SetGraphicsRootSignature(RootSignature);
}