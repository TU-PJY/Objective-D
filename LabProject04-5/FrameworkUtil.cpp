#include "FrameworkUtil.h"
#include "ResourceManager.h"

// 이 프로젝트의 핵심 유틸이다. 프로그램의 모든 객체의 업데이트 및 렌더링은 모두 이 프레임워크를 거친다.

// 프레임워크를 초기화 한다. 실행 시 단 한 번만 실행되는 함수로, 더미 객체를 추가한 후 모드를 시작한다.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, const char* ModeFunction()) {
	// 필요한 파일들을 모두 로드한다
	RootSignature = CreateGraphicsRootSignature(Device);
	LoadShaderResource(RootSignature, Device, CmdList);

	// 업데이트를 담당하는 컨테이너에는 추가되나, 객체 삭제, 객체 검색등을 담당하는 멀티맵에는 추가되지 않는다.
	for (int i = 0; i < NUM_LAYER; ++i)
		Container[i].push_back(new DUMMY);

	SwitchMode(ModeFunction);
}

// 현재 실행 중인 모드 이름을 리턴한다
const char* Framework::Mode() {
	return RunningMode;
}

// 모드를 변경한다. 모드 변경 시 기존 모드에 있던 객체들은 모두 삭제된다.
void Framework::SwitchMode(const char* ModeFunction()) {
	ClearAll();
	RunningMode = ModeFunction();
}

// 컨트롤러 설정 함수이다. 이 함수를 직접 작성할 일은 없다,
void Framework::SetKeyController(void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardController = KeyboardControllerPtr;
}
void Framework::SetMouseController(void (*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseController = MouseControllePtr;
}
void Framework::SetMouseMotionController(void (*MouseMotionControllerPtr)(HWND)) {
	MouseMotionController = MouseMotionControllerPtr;
}

// 키모드, 마우스, 마우스 움직임을 객체 포인터에 전달한다. 이 함수들 역시 직접 쓸 일은 없다.
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

// 프로그램을 종료한다.
void Framework::Exit() {
	PostQuitMessage(1);
}

// 현재 존재하는 모든 객체들을 업데이트한다
// 삭제 마크가 표시된 객체를 업데이트되지 않는다
void Framework::Update(float FT) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}

		// 업데이트를 모두 마친 후 업데이트 중이었던 레이어에서 삭제 마크가 표시된 객체를 찾아 삭제한다.
		UpdateContainer(i);
	}
}

// 현재 존재하는 모든 객체들을 렌더링한다
// 삭제 마크가 표시된 객체들은 렌더링되지 않는다.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < NUM_LAYER; ++i) {
		for (auto const& O : Container[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

// 객체를 추가한다. 원하는 객체와 태그, 레이어를 설정할 수 있다.
// 이 함수에서 입력한 태그는 Find()함수에서 사용된다.
void Framework::AddObject(GameObject*&& Object, const char* Tag, Layer Layer) {
	int layer = static_cast<int>(Layer);

	Container[layer].push_back(Object);
	Container[layer].back()->ObjectTag = Tag;
	ObjectList.insert(std::pair(Tag, Container[layer].back()));
}

// 포인터를 사용하여 객체를 삭제한다. 객체에 삭제 마크를 표시한다. 
// 이 코드가 실행되는 시점에 즉시 삭제되지 않음에 유의한다. 
// 삭제 마크가 표시된 객체는 UpdateContainer()에서 최종적으로 삭제된다.
// 클래스 내부에서 this 포인터로도 자신을 삭제할 수 있다.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// 현재 존재하는 객체들 중 특정 객체의 포인터를 얻어 접근할 때 사용한다.
// 이진 탐색을 사용하여 검색하므로 매우 빠르다.
GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// 특정 레이어에 존재하는 다수의 객체들의 포인터를 얻어 접근하는데에 사용한다.
// for문과 함께 사용하야 한다.
GameObject* Framework::Find(const char* Tag, Layer TargetLayer, int Index) {
	int layer = static_cast<int>(TargetLayer);

	if (Container[layer][Index]->ObjectTag == Tag)
		return Container[layer][Index];
	else
		return nullptr;
}

// 현재 존재하는 모든 객체들을 삭제한다. 더미 객체는 ObjectList에 있지 않으므로 삭제 마크가 표시되지 않늗다.
void Framework::ClearAll() {
	for (const auto& O : ObjectList)
		O.second->DeleteMark = true;
}

// 특정 레이어의 오브젝트 개수를 리턴한다. 
// GameObject* Framework::Find(const char* Tag, Layer TargetLayer, int Index) 함수를 사용하기 위해서는 이 함수가 필요하다.
size_t Framework::ObjectCount(Layer TargetLayer) {
	return Container[(int)TargetLayer].size();
}

// 삭제 마크가 표시된 객체들을 삭제한다.
void Framework::UpdateContainer(int Index) {
	std::erase_if(ObjectList, [](const std::pair<const char*, GameObject*>& Object) {
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

/////////////////////

// 루트 시그니처를 생성한다
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

// 루트 시그니처를 리턴한다
ID3D12RootSignature* Framework::GetGraphicsRootSignature() {
	return(RootSignature);
}

// 루트시그니처를 릴리즈한다
void Framework::ReleaseObjects() {
	if (RootSignature)
		RootSignature->Release();
}

// 렌더링을 준비한다
void Framework::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	CmdList->SetGraphicsRootSignature(RootSignature);
}