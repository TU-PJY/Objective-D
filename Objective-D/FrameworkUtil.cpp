#include "FrameworkUtil.h"
#include "ResourceManager.h"
#include <array>

// 이 프로젝트의 핵심 유틸이다. 프로그램의 모든 객체의 업데이트 및 렌더링은 모두 이 프레임워크를 거친다.

// 프레임워크를 초기화 한다. 실행 시 단 한 번만 실행되는 함수로, 더미 객체를 추가한 후 모드를 시작한다.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// 루트 시그니처를 생성한다.
	RootSignature = CreateGraphicsRootSignature(Device);

	// 전역 쉐이더를 생성한다.
	CreateShaderResource(RootSignature, Device, CmdList);

	// 전역 매쉬를 생성한다.
	CreateMeshResource(Device, CmdList);

	// 전역 텍스처를 생성한다
	CreateTextureResource(Device, CmdList);

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

// 키보드, 마우스, 마우스 움직임을 객체 포인터에 전달한다. 이 함수들 역시 직접 쓸 일은 없다.
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
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}
		UpdateObjectList(i);
	}
}

// 현재 존재하는 모든 객체들을 렌더링한다
// 삭제 마크가 표시된 객체들은 렌더링되지 않는다.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

// 객체를 추가한다. 원하는 객체와 태그, 레이어를 설정할 수 있다.
// 이 함수에서 입력한 태그는 Find()함수에서 사용된다.
void Framework::AddObject(GameObject*&& Object, const char* Tag, Layer InputLayer) {
	int TargetLayer = (int)InputLayer;
	ObjectList[TargetLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));
	Object->ObjectTag = Tag;
}

// 포인터를 사용하여 객체를 삭제한다. 객체에 삭제 마크를 표시한다. 
// 이 코드가 실행되는 시점에 즉시 삭제되지 않음에 유의한다. 
// 삭제 마크가 표시된 객체는 UpdateObjectIndex()에서 최종적으로 삭제된다.
// 클래스 내부에서 this 포인터로도 자신을 삭제할 수 있다.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// 현재 존재하는 객체들 중 특정 객체의 포인터를 얻어 접근할 때 사용한다.
// 이진 탐색을 사용하여 검색하므로 매우 빠르다.
GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != std::end(ObjectIndex) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// 특정 태그를 가진 오브젝트들의 포인터 범위를 리턴한다.
// 해당 함수로 equal range를 얻어 for문으로 접근하면 된다.
ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range{};

	auto It = ObjectIndex.equal_range(Tag);
	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

// 현재 존재하는 모든 객체들을 삭제한다.
void Framework::ClearAll() {
	for (const auto& O : ObjectIndex)
		O.second->DeleteMark = true;
}

// 삭제 마크가 표시된 객체들을 컨테이너에서 삭제한다.
// 실제 객체 삭제가 아님에 유의
void Framework::UpdateObjectList(int Index) {
	for (auto It = begin(ObjectList[Index]); It != end(ObjectList[Index]);) {
		if ((*It)->DeleteMark) {
			It = ObjectList[Index].erase(It);
			continue;
		}
		++It;
	}
}

// 삭제 마크가 표시된 객체들을 실제로 삭제한다.
void Framework::UpdateObjectIndex() {
	for (auto It = begin(ObjectIndex); It != end(ObjectIndex);) {
		if (It->second->DeleteMark) {
			delete It->second;
			It->second = nullptr;
			It = ObjectIndex.erase(It);
			continue;
		}
		++It;
	}
}


/////////////////////

void SetRoot(std::vector<D3D12_ROOT_PARAMETER>& RootParam, int NumValue, int RegisterNum, int RootIndex) {
	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParam[RootIndex].Constants.Num32BitValues = NumValue;
	RootParam[RootIndex].Constants.ShaderRegister = RegisterNum;
	RootParam[RootIndex].Constants.RegisterSpace = 0;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
}

void SetCBV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
}

void SetSRV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
}

void SetSampler(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
}


// 루트 시그니처를 생성한다
ID3D12RootSignature* Framework::CreateGraphicsRootSignature(ID3D12Device* Device) {
	ID3D12RootSignature* GraphicsRootSignature = NULL;

	// 32비트 상수들을 정의
	D3D12_DESCRIPTOR_RANGE Range{};
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(9, {});
	int RootParameterNum = RootParameters.size();

	// srv, sampler 인덱스는 항상 맨 끝에 위치하도록 한다
	SRV_INDEX_NUMBER = RootParameterNum - 2;
	SAMPLER_INDEX_NUMBER = RootParameterNum - 1;

	SetRoot(RootParameters, 0, 0, 0);  // b0, 미사용이라서 루트상수 미할당
	SetRoot(RootParameters, 19, 1, 1);  // b1
	SetRoot(RootParameters, 35, 2, 2);  // b2

	SetCBV(Range, RootParameters, 3, 3); // b3, cbFlipInfo
	CBVUtil::SetSignatureIndex(FlipCBV, 3);

	SetRoot(RootParameters, 1, 4, 4); // b4

	SetCBV(Range, RootParameters, 5, 5); // b5, cbLightInfo
	CBVUtil::SetSignatureIndex(LightCBV, 5);

	SetCBV(Range, RootParameters, 6, 6); // b6,  cbLightUseInfo
	CBVUtil::SetSignatureIndex(BoolLightCBV, 6);

	SetSRV(Range, RootParameters, 0, SRV_INDEX_NUMBER);  // t0
	SetSampler(Range, RootParameters, 0, SAMPLER_INDEX_NUMBER); // s0

	// 루트 시그니처 설정
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
	RootSignatureDesc.NumParameters = RootParameters.size(); //_countof(RootParameters);
	RootSignatureDesc.pParameters = RootParameters.data();
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