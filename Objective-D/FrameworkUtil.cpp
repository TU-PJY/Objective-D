#include "FrameworkUtil.h"
#include "ResourceManager.h"
#include <array>

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ���̴��� �ε��ϰ� ���� ��带 �����Ѵ�
	RootSignature = CreateGraphicsRootSignature(Device);
	LoadShaderResource(RootSignature, Device, CmdList);

	// ���� �Ž��� �� �Լ��ּ� �ε��Ѵ�.
	LoadMeshResource(Device, CmdList);

	// ���� �ؽ�ó�� �� �Լ����� �ε��Ѵ�.
	LoadTextureResource(Device, CmdList);

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
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateContainer()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Framework::DeleteObject(GameObject* Object) {
	Object->DeleteMark = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// �ؽ� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
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

/////////////////////

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Framework::CreateGraphicsRootSignature(ID3D12Device* Device) {
	ID3D12RootSignature* GraphicsRootSignature = NULL;

	// 32��Ʈ ������� ����
	std::array<D3D12_ROOT_PARAMETER, 7> RootParameters;
	int RootParameterNum = RootParameters.size();

	// srv, sampler �ε����� �׻� �� ���� ��ġ�ϵ��� �Ѵ�
	SRV_INDEX_NUMBER = RootParameterNum - 2;
	SAMPLER_INDEX_NUMBER = RootParameterNum - 1;

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

	// b4
	D3D12_DESCRIPTOR_RANGE cbvRange2;
	cbvRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	cbvRange2.NumDescriptors = 1;
	cbvRange2.BaseShaderRegister = 4;
	cbvRange2.RegisterSpace = 0;
	cbvRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[4].DescriptorTable.pDescriptorRanges = &cbvRange2;
	RootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// t0
	D3D12_DESCRIPTOR_RANGE srvRange;
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.NumDescriptors = 1;
	srvRange.BaseShaderRegister = 0; // t0
	srvRange.RegisterSpace = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[SRV_INDEX_NUMBER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[SRV_INDEX_NUMBER].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[SRV_INDEX_NUMBER].DescriptorTable.pDescriptorRanges = &srvRange;
	RootParameters[SRV_INDEX_NUMBER].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	// s0
	D3D12_DESCRIPTOR_RANGE samplerRange;
	samplerRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
	samplerRange.NumDescriptors = 1;
	samplerRange.BaseShaderRegister = 0; // s0
	samplerRange.RegisterSpace = 0;
	samplerRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	RootParameters[SAMPLER_INDEX_NUMBER].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	RootParameters[SAMPLER_INDEX_NUMBER].DescriptorTable.NumDescriptorRanges = 1;
	RootParameters[SAMPLER_INDEX_NUMBER].DescriptorTable.pDescriptorRanges = &samplerRange;
	RootParameters[SAMPLER_INDEX_NUMBER].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

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