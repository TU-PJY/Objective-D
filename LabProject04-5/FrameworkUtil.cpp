#include "FrameworkUtil.h"
#include "ResourceManager.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ���̴��� �ε��ϰ� ���� ��带 �����Ѵ�
	RootSignature = CreateGraphicsRootSignature(Device);
	LoadShaderResource(RootSignature, Device, CmdList);
	ModeFunction();
}

// ���� ���� ���� ��� �̸��� �����Ѵ�
const char* Framework::GetMode() {
	return RunningMode;
}

void Framework::RegisterDestructor(Function Destructor) {
	DestructorBuffer = Destructor;
}

void Framework::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

// ��带 �����Ѵ�. ��� ���� �� ���� ��忡 �ִ� ��ü���� ��� �����ȴ�.
void Framework::SwitchMode(Function ModeFunction) {
	ClearAll();
	if (DestructorBuffer)
		DestructorBuffer();
	ModeFunction();
}

void Framework::RegisterModeName(const char* ModeName) {
	RunningMode = ModeName;
}

// ��Ʈ�ѷ� ���� �Լ��̴�. �� �Լ��� ���� �ۼ��� ���� ����,
void Framework::RegisterKeyController(void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardController = KeyboardControllerPtr;
}
void Framework::RegisterMouseController(void (*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseController = MouseControllePtr;
}
void Framework::RegisterMouseMotionController(void (*MouseMotionControllerPtr)(HWND)) {
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
	for (auto const& O : ObjectList) {
		if (!O.second->DeleteMark)
			O.second->Update(FT);
	}
}

// ���� �����ϴ� ��� ��ü���� �������Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü���� ���������� �ʴ´�.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (auto const& O : ObjectList) {
		if (!O.second->DeleteMark)
			O.second->Render(CmdList);
	}
}

// ���� ��ũ�� ǥ�õ� ��ü���� �����Ѵ�.
void Framework::UpdateContainer() {
	for (auto It = begin(ObjectList); It != end(ObjectList);) {
		if (It->second->DeleteMark) {
			delete It->second;
			It->second = nullptr;
			It = ObjectList.erase(It);
			continue;
		}
		++It;
	}
}

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
// �� �Լ����� �Է��� �±״� Find()�Լ����� ���ȴ�.
void Framework::AddObject(GameObject*&& Object, const char* Tag) {
	ObjectList.insert(std::pair(Tag, Object));
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�. 
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�. 
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateContainer()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// �ؽ� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// Ư�� �±׸� ���� ������Ʈ���� ������ ������ �����Ѵ�.
// �ش� �Լ��� equal range�� ��� for������ �����ϸ� �ȴ�.
ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range{};

	auto It = ObjectList.equal_range(Tag);
	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�.
void Framework::ClearAll() {
	for (const auto& O : ObjectList)
		O.second->DeleteMark = true;
}

/////////////////////

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