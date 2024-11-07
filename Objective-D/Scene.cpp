#include "Scene.h"
#include "GameResource.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "MouseUtil.h"

// �� ������Ʈ�� �ٽ� ��ƿ�̴�. ���α׷��� ��� ��ü�� ������Ʈ �� �������� ��� �� �����ӿ�ũ�� ��ģ��.

// �����ӿ�ũ�� �ʱ�ȭ �Ѵ�. ���� �� �� �� ���� ����Ǵ� �Լ���, ���� ��ü�� �߰��� �� ��带 �����Ѵ�.
void Scene::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction) {
	// ��Ʈ �ñ״�ó�� �����Ѵ�.
	RootSignature = CreateGraphicsRootSignature(Device);

	// ���� ���̴��� �����Ѵ�.
	CreateShaderResource(RootSignature, Device, CmdList);

	// ���� �Ž��� �����Ѵ�.
	CreateMeshResource(Device, CmdList);

	// ���� �ؽ�ó�� �����Ѵ�
	CreateTextureResource(Device, CmdList);

	// ���� ��� �Լ� ����
	ModeFunction();
}

// ���� ���� ���� ��� �̸��� �����Ѵ�
const char* Scene::GetMode() {
	return RunningMode;
}

// ��� �Ҹ��ڸ� ����Ѵ�.
void Scene::RegisterDestructor(Function Destructor) {
	DestructorBuffer = Destructor;
}

// ��� �Ҹ��� �����͸� ���´�.
void Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

// ���� �����ϴ� ��� ��ü���� ������Ʈ�ϰ� �������Ѵ�.
// ���� ��ũ�� ǥ�õ� ��ü���� ������Ʈ ����Ʈ���� �����ȴ�. (���� ��ü ������ �ƴ�)
void Scene::Routine(float FT, ID3D12GraphicsCommandList* CmdList) {
	ObjectCmdList = CmdList;

	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectList[i]); It != end(ObjectList[i]);) {
			if (!(*It)->ObjectDeleteCommand) {
				(*It)->Update(FT);
				(*It)->Render();
			}

			else {
				It = ObjectList[i].erase(It);
				continue;
			}

			++It;
		}
	}
}

// ��带 �����Ѵ�. ��� ���� �� ���� scene�� �ִ� ��ü���� ��� �����ȴ�.
void Scene::SwitchMode(Function ModeFunction) {
	ClearAll();
	if (DestructorBuffer)
		DestructorBuffer();
	ModeFunction();
}

// ��� �̸��� ����Ѵ�. �ߺ��Ǵ� ��� �̸��� ������� �ʵ��� �����Ѵ�.
void Scene::RegisterModeName(const char* ModeName) {
	RunningMode = ModeName;
}

// ��Ʈ�ѷ� ���� �Լ��̴�. �� �Լ��� ���� �ۼ��� ���� ����,
void Scene::RegisterKeyController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	KeyboardControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseController(void (*FunctionPtr)(HWND, UINT, WPARAM, LPARAM)) {
	MouseControllerPtr = FunctionPtr;
}
void Scene::RegisterMouseMotionController(void (*FunctionPtr)(HWND)) {
	MouseMotionControllerPtr = FunctionPtr;
}

// ��ü�� ã�� ��Ʈ�ѷ� �Լ��� �޽����� �����Ѵ�. �ټ��� �����ϴ� ��ü�� ������� �ʵ��� �����Ѵ�.
void Scene::InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->ObjectDeleteCommand)
		It->second->InputMouse(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->ObjectDeleteCommand)
		It->second->InputKey(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMotion(HWND hWnd,  const char* ObjectTag) {
	auto It = ObjectIndex.find(ObjectTag);
	if (It != end(ObjectIndex) && !It->second->ObjectDeleteCommand)
		It->second->InputMouseMotion(hWnd, mouse.MotionPosition);
}

// ��ü�� �߰��Ѵ�. ���ϴ� ��ü�� �±�, ���̾ ������ �� �ִ�.
// �� �Լ����� �Է��� �±״� Find()�Լ����� ���ȴ�.
void Scene::AddObject(GameObject*&& Object, const char* Tag, int InputLayer) {
	ObjectList[InputLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));
	Object->ObjectTag = Tag;
}

// �����͸� ����Ͽ� ��ü�� �����Ѵ�. ��ü�� ���� ��ũ�� ǥ���Ѵ�.
// �� �ڵ尡 ����Ǵ� ������ ��� �������� ������ �����Ѵ�.
// ���� ��ũ�� ǥ�õ� ��ü�� UpdateObjectIndex()���� ���������� �����ȴ�.
// Ŭ���� ���ο��� this �����ͷε� �ڽ��� ������ �� �ִ�.
void Scene::DeleteObject(GameObject* Object) {
	Object->ObjectDeleteCommand = true;
}

// ���� �����ϴ� ��ü�� �� Ư�� ��ü�� �����͸� ��� ������ �� ����Ѵ�.
// ���� Ž���� ����Ͽ� �˻��ϹǷ� �ſ� ������.
GameObject* Scene::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != std::end(ObjectIndex) && !It->second->ObjectDeleteCommand)
		return It->second;
	else
		return nullptr;
}

// Ư�� �±׸� ���� ������Ʈ���� ������ ������ �����Ѵ�.
// �ش� �Լ��� equal range�� ��� for������ �����ϸ� �ȴ�.
std::pair<ObjectRange, ObjectRange> Scene::EqualRange(const char* Tag) {
	return ObjectIndex.equal_range(Tag);
}

// ���� �����ϴ� ��� ��ü���� �����Ѵ�.
void Scene::ClearAll() {
	for (const auto& O : ObjectIndex)
		O.second->ObjectDeleteCommand = true;
}

// ���� ��ũ�� ǥ�õ� ��ü���� ������ �����Ѵ�.
void Scene::ProcessIndexCommand() {
	for (auto It = begin(ObjectIndex); It != end(ObjectIndex);) {
		if (It->second->ObjectDeleteCommand) {
			delete It->second;
			It->second = nullptr;
			It = ObjectIndex.erase(It);
			continue;
		}
		++It;
	}
}

// ���α׷��� �����Ѵ�.
void Scene::Exit() {
	PostQuitMessage(1);
}


// Ű����, ���콺, ���콺 �������� WinMain���κ��� �޾ƿ´�. ���� �� ���� ����.
void Scene::InputKeyMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (KeyboardControllerPtr)
		KeyboardControllerPtr(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (MouseControllerPtr)
		MouseControllerPtr(hWnd, nMessageID, wParam, lParam);
}
void Scene::InputMouseMotionMessage(HWND hWnd) {
	if (MouseMotionControllerPtr)
		MouseMotionControllerPtr(hWnd);
}

// ��Ʈ �ñ״�ó�� �����Ѵ�
ID3D12RootSignature* Scene::GetGraphicsRootSignature() {
	return(RootSignature);
}

// ��Ʈ�ñ״�ó�� �������Ѵ�
void Scene::ReleaseObjects() {
	if (RootSignature)
		RootSignature->Release();
}

// �������� �غ��Ѵ�
void Scene::PrepareRender(ID3D12GraphicsCommandList* CmdList) {
	CmdList->SetGraphicsRootSignature(RootSignature);
}