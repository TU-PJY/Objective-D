#include "FrameworkUtil.h"
#include "ResourceManager.h"
#include "CBVUtil.h"
#include "RootConstants.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ��Ʈ �ñ״�ó�� �����Ѵ�.
	RootSignature = CreateGraphicsRootSignature(Device);

	// ���� ���̴��� �����Ѵ�.
	CreateShaderResource(RootSignature, Device, CmdList);

	// ���� �Ž��� �����Ѵ�.
	CreateMeshResource(Device, CmdList);

	// ���� �ؽ�ó�� �����Ѵ�
	CreateTextureResource(Device, CmdList);

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

// Ű����, ���콺, ���콺 �������� ��ü �����Ϳ� �����Ѵ�. �� �Լ��� ���� ���� �� ���� ����.
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
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Update(FT);
		}
		UpdateObjectList(i);
	}
}

// ���� �����ϴ� ��� ��ü���� �������Ѵ�
// ���� ��ũ�� ǥ�õ� ��ü���� ���������� �ʴ´�.
void Framework::Render(ID3D12GraphicsCommandList* CmdList) {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteMark)
				O->Render(CmdList);
		}
	}
}

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
// �� �Լ����� �Է��� �±״� Find()�Լ����� ���ȴ�.
void Framework::AddObject(GameObject*&& Object, const char* Tag, Layer InputLayer) {
	int TargetLayer = (int)InputLayer;
	ObjectList[TargetLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));
	Object->ObjectTag = Tag;
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�.
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�.
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateObjectIndex()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// ���� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Framework::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != std::end(ObjectIndex) && !It->second->DeleteMark)
		return It->second;
	else
		return nullptr;
}

// Ư�� �±׸� ���� ������Ʈ���� ������ ������ �����Ѵ�.
// �ش� �Լ��� equal range�� ��� for������ �����ϸ� �ȴ�.
ObjectRange Framework::EqualRange(const char* Tag) {
	ObjectRange Range{};

	auto It = ObjectIndex.equal_range(Tag);
	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�.
void Framework::ClearAll() {
	for (const auto& O : ObjectIndex)
		O.second->DeleteMark = true;
}

// ���� ��ũ�� ǥ�õ� ��ü���� �����̳ʿ��� �����Ѵ�.
// ���� ��ü ������ �ƴԿ� ����
void Framework::UpdateObjectList(int Index) {
	for (auto It = begin(ObjectList[Index]); It != end(ObjectList[Index]);) {
		if ((*It)->DeleteMark) {
			It = ObjectList[Index].erase(It);
			continue;
		}
		++It;
	}
}

// ���� ��ũ�� ǥ�õ� ��ü���� ������ �����Ѵ�.
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

void SetRoot(std::vector<D3D12_ROOT_PARAMETER>& RootParam, int NumValue, int RegisterNum, int RootIndex, int& IndexValue) {
	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParam[RootIndex].Constants.Num32BitValues = NumValue;
	RootParam[RootIndex].Constants.ShaderRegister = RegisterNum;
	RootParam[RootIndex].Constants.RegisterSpace = 0;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	IndexValue = RootIndex;
}

void SetCBV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, CBV& CBV_Struct) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	CBVUtil::SetSignatureIndex(CBV_Struct, RootIndex);
}

void SetSRV(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, int& IndexValue) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	IndexValue = RootIndex;
}

void SetSampler(D3D12_DESCRIPTOR_RANGE Range, std::vector<D3D12_ROOT_PARAMETER>& RootParam, int RegisterNum, int RootIndex, int& IndexValue) {
	Range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	Range.NumDescriptors = 1;
	Range.BaseShaderRegister = RegisterNum;
	Range.RegisterSpace = 0;
	Range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParam[RootIndex].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParam[RootIndex].DescriptorTable.NumDescriptorRanges = 1;
	RootParam[RootIndex].DescriptorTable.pDescriptorRanges = &Range;
	RootParam[RootIndex].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	IndexValue = RootIndex;
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Framework::CreateGraphicsRootSignature(ID3D12Device* Device) {
	ID3D12RootSignature* GraphicsRootSignature = NULL;

	// 32��Ʈ ������� ����
	D3D12_DESCRIPTOR_RANGE Range{};

	// ��Ʈ �Ķ����
	std::vector<D3D12_ROOT_PARAMETER> RootParameters(9);

	int Dummy{};
	SetRoot(RootParameters, 0, 0, 0, Dummy);                // b0, cbFrameworkInfo | �̻���̶� ��Ʈ��� ���Ҵ�

	SetRoot(RootParameters, 19, 1, 1, GAME_OBJECT_INDEX);   // b1, cbGameObjectInfo

	SetRoot(RootParameters, 35, 2, 2, CAMERA_INDEX);        // b2, cbCameraInfo

	SetCBV(Range, RootParameters, 3, 3, FlipCBV);           // b3, cbFlipInfo

	SetRoot(RootParameters, 1, 4, 4, ALPHA_INDEX);          // b4, cbAlphaInfo

	SetCBV(Range, RootParameters, 5, 5, LightCBV);          // b5, cbLightInfo

	SetCBV(Range, RootParameters, 6, 6, BoolLightCBV);      // b6, cbLightUseInfo

	SetSRV(Range, RootParameters, 0, 7, SRV_INDEX);         // t0, SRV

	SetSampler(Range, RootParameters, 0, 8, SAMPLER_INDEX); // s0, Sampler

	// ��Ʈ �ñ״�ó ����
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