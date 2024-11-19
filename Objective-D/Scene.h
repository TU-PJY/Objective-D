#pragma once
#include "Config.h"
#include "GameObject.h"
#include "ShaderUtil.h"
#include <deque>
#include <array>
#include <map>
#include <vector>

typedef void(*Function)(void);
using ObjectRange = std::multimap<const char*, GameObject*>::iterator;
constexpr int Layers = static_cast<int>(END);

class Scene {
private:
	std::array<std::deque<GameObject*>, Layers> ObjectList{};
	std::multimap<const char*, GameObject*> ObjectIndex{};
	std::array<std::vector<int>, Layers> DeleteLocation{};
	int CurrentReferPosition{};
	int SceneCommandCount{};
	bool CommandExist{};

	const char* RunningMode{};
	void (*MouseControllerPtr)(HWND, UINT, WPARAM, LPARAM) {};
	void (*MouseMotionControllerPtr)(HWND) {};
	void (*KeyboardControllerPtr)(HWND, UINT, WPARAM, LPARAM) {};

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
	void RegisterKeyController(void(*FunctionPtr)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseController(void(*FunctionPtr)(HWND, UINT, WPARAM, LPARAM));
	void RegisterMouseMotionController(void(*FunctionPtr)(HWND));

	void InputKeyMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotionMessage(HWND hWnd);
	void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag);
	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, const char* ObjectTag);
	void InputMouseMotion(HWND hWnd, const char* ObjectTag);
	void Routine(float FT, ID3D12GraphicsCommandList* CmdList);
	void Exit();
	void AddObject(GameObject*&& Object, const char* Tag, int InputLayer);
	void DeleteObject(GameObject* Object);
	GameObject* Find(const char* Tag);
	std::pair<ObjectRange, ObjectRange> EqualRange(const char* Tag);
	void CompleteCommand();
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void ReleaseObjects();
	void PrepareRender(ID3D12GraphicsCommandList* CmdList);

private:
	void AddLocation(int Layer, int Position);
	void ProcessObjectCommand();
	void ProcessSceneCommand();
	void ClearAll();
};

// global scope scene
extern Scene scene;