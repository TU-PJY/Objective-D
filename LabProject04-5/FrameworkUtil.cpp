#include "FrameworkUtil.h"
#include "ResourceManager.h"

// 이 프로젝트의 핵심 유틸이다. 프로그램의 모든 객체의 업데이트 및 렌더링은 모두 이 프레임워크를 거친다.

// 프레임워크를 초기화 한다. 실행 시 단 한 번만 실행되는 함수로, 더미 객체를 추가한 후 모드를 시작한다.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// 쉐이더를 로드하고 시작 모드를 실행한다
	RootSignature = CreateGraphicsRootSignature(Device);
	LoadShaderResource(RootSignature, Device, CmdList);

	// 전역 매쉬를 이 함수애서 로드한다.
	LoadMeshResource(Device, CmdList);

	// 전역 텍스처를 이 함수에서 로드한다.
	LoadTextureResource(Device, CmdList);

	ModeFunction();
}

// 현재 실행 중인 모드 이름을 리턴한다
const char* Framework::GetMode() {
	return RunningMode;
}

void Framework::RegisterDestructor(Function Destructor) {
	DestructorBuffer = Destructor;
}

void Framework::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

// 모드를 변경한다. 모드 변경 시 기존 모드에 있던 객체들은 모두 삭제된다.
void Framework::SwitchMode(Function ModeFunction) {
	ClearAll();
	if (DestructorBuffer)
		DestructorBuffer();
	ModeFunction();
}

void Framework::RegisterModeName(const char* ModeName) {
	RunningMode = ModeName;
}

// 컨트롤러 설정 함수이다. 이 함수를 직접 작성할 일은 없다,
void Framework::RegisterKeyController(void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardController = KeyboardControllerPtr;
}
void Framework::RegisterMouseController(void (*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseController = MouseControllePtr;
}
void Framework::RegisterMouseMotionController(void (*MouseMotionControllerPtr)(HWND)) {
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
	for (auto const& O : ObjectList) {
		if (!O.second->DeleteMark)
			O.second->Update(FT);
	}
}

// 현재 존재하는 모든 객체들을 렌더링한다
// 삭제 마크가 표시된 객체들은 렌더링되지 않는다.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (auto const& O : ObjectList) {
		if (!O.second->DeleteMark)
			O.second->Render(CmdList);
	}
}

// 삭제 마크가 표시된 객체들을 삭제한다.
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

// 객체를 추가한다. 원하는 객체와 태그, 레이어를 설정할 수 있다.
// 이 함수에서 입력한 태그는 Find()함수에서 사용된다.
void Framework::AddObject(GameObject*&& Object, const char* Tag) {
	ObjectList.insert(std::pair(Tag, Object));
}

// 포인터를 사용하여 객체를 삭제한다. 객체에 삭제 마크를 표시한다. 
// 이 코드가 실행되는 시점에 즉시 삭제되지 않음에 유의한다. 
// 삭제 마크가 표시된 객체는 UpdateContainer()에서 최종적으로 삭제된다.
// 클래스 내부에서 this 포인터로도 자신을 삭제할 수 있다.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// 현재 존재하는 객체들 중 특정 객체의 포인터를 얻어 접근할 때 사용한다.
// 해쉬 탐색을 사용하여 검색하므로 매우 빠르다.
GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectList.find(Tag);
	if (It != std::end(ObjectList) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// 특정 태그를 가진 오브젝트들의 포인터 범위를 리턴한다.
// 해당 함수로 equal range를 얻어 for문으로 접근하면 된다.
ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range{};

	auto It = ObjectList.equal_range(Tag);
	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

// 현재 존재하는 모든 객체들을 삭제한다.
void Framework::ClearAll() {
	for (const auto& O : ObjectList)
		O.second->DeleteMark = true;
}

/////////////////////

// 루트 시그니처를 생성한다
ID3D12RootSignature* Framework::CreateGraphicsRootSignature(ID3D12Device* Device) {
	ID3D12RootSignature* GraphicsRootSignature = NULL;

	// 32비트 상수들을 정의
	D3D12_ROOT_PARAMETER RootParameters[6];

	// b0
	RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParameters[0].Constants.Num32BitValues = 4;
	RootParameters[0].Constants.ShaderRegister = 0;
	RootParameters[0].Constants.RegisterSpace = 0;
	RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// b1
	RootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParameters[1].Constants.Num32BitValues = 19;
	RootParameters[1].Constants.ShaderRegister = 1;
	RootParameters[1].Constants.RegisterSpace = 0;
	RootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// b2
	RootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParameters[2].Constants.Num32BitValues = 35;
	RootParameters[2].Constants.ShaderRegister = 2;
	RootParameters[2].Constants.RegisterSpace = 0;
	RootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// b3
	D3D12_DESCRIPTOR_RANGE cbvRange;
	cbvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbvRange.NumDescriptors = 1;
	cbvRange.BaseShaderRegister = 3;
	cbvRange.RegisterSpace = 0;
	cbvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[3].DescriptorTable.pDescriptorRanges = &cbvRange;
	RootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// t0
	D3D12_DESCRIPTOR_RANGE srvRange;
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.NumDescriptors = 1;
	srvRange.BaseShaderRegister = 0; // t0
	srvRange.RegisterSpace = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[4].DescriptorTable.pDescriptorRanges = &srvRange;
	RootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// s0
	D3D12_DESCRIPTOR_RANGE samplerRange;
	samplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	samplerRange.NumDescriptors = 1;
	samplerRange.BaseShaderRegister = 0; // s0
	samplerRange.RegisterSpace = 0;
	samplerRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[5].DescriptorTable.pDescriptorRanges = &samplerRange;
	RootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// 루트 시그니처 설정
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
	RootSignatureDesc.NumParameters = _countof(RootParameters);
	RootSignatureDesc.pParameters = RootParameters;
	RootSignatureDesc.NumStaticSamplers = 0;
	RootSignatureDesc.pStaticSamplers = NULL;
	RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* SignatureBlob = NULL;
	ID3DBlob* ErrorBlob = NULL;
	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SignatureBlob, &ErrorBlob);
	Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&GraphicsRootSignature);

	if (SignatureBlob) SignatureBlob->Release();
	if (ErrorBlob) ErrorBlob->Release();

	return GraphicsRootSignature;
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