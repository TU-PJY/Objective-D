#pragma once
#include "Config.h"
#include "GameObject.h"
#include "ShaderUtil.h"
#include <vector>
#include <array>
#include <map>

typedef void(*Function)(void);
using LayerIter = std::map<const char*, GameObject*>::iterator;
constexpr int Layers = static_cast<int>(Layer::END);

typedef struct {
	LayerIter First, End;
}ObjectRange;

class Framework {
private:
	//std::unordered_multimap<const char*, GameObject*> ObjectList;
	std::array<std::vector<GameObject*>, Layers> ObjectList;
	std::map<const char*, GameObject*> ObjectIndex;

	const char* RunningMode{};
	void (*MouseController)(HWND, UINT, WPARAM, LPARAM);
	void (*MouseMotionController)(HWND);
	void (*KeyboardController)(HWND, UINT, WPARAM, LPARAM);
	Function DestructorBuffer{};

protected:
	ID3D12RootSignature* RootSignature{};

public:
	const char* GetMode();
	void RegisterDestructor(Function Destructor);
	void ReleaseDestructor();
	void Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, Function ModeFunction);
	void SwitchMode(Function ModeFunction);
	void RegisterModeName(const char* ModeName);
	void RegisterKeyController(void(*KeyboardController)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseController(void(*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseMotionController(void(*MouseMotionControllerPtr)(HWND));

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotion(HWND hWnd);
	void Exit();
	void Update(float FT);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void UpdateObjectList(int Index);
	void UpdateObjectIndex();
	void AddObject(GameObject*&& Object, const char* Tag, Layer InputLayer);
	void DeleteObject(GameObject* Object);
	GameObject* Find(const char* Tag);
	ObjectRange EqualRange(const char* Tag);
	void ClearAll();
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void ReleaseObjects();
	void PrepareRender(ID3D12GraphicsCommandList* CmdList);
};

// global scope framework
extern Framework framework;