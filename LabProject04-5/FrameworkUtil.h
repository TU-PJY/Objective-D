#pragma once
#include "Config.h"
#include "GameObject.h"
#include "ShaderUtil.h"
#include <deque>
#include <ranges>
#include <map>

// global scope shader
constexpr int NUM_LAYER = static_cast<int>(Layer::END);

class Framework {
private:
	std::multimap<const char*, GameObject*> ObjectList;

	const char* RunningMode{};
	void (*MouseController)(HWND, UINT, WPARAM, LPARAM);
	void (*MouseMotionController)(HWND);
	void (*KeyboardController)(HWND, UINT, WPARAM, LPARAM);

protected:
	ID3D12RootSignature* RootSignature{};
	std::array<std::deque<GameObject*>, NUM_LAYER> Container;

public:
	const char* Mode();
	POINT PrevCursorPos{};
	bool LB_DownState{}, RB_DownState{};

	void Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, const char* ModeFunction());

	void SwitchMode(const char* ModeFunction());

	void SetKeyController(void(*KeyboardController)(HWND, UINT, WPARAM, LPARAM));
	void SetMouseController(void(*MouseControllePtr)(HWND, UINT, WPARAM, LPARAM));
	void SetMouseMotionController(void(*MouseMotionControllerPtr)(HWND));

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotion(HWND hWnd);
	void Exit();
	void Update(float FT);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void UpdateContainer(int Index);
	void AddObject(GameObject*&& Object, const char* Tag, Layer Layer);
	void DeleteObject(GameObject* Object);
	void DeleteObject(const char* Tag);
	void DeleteObject(const char* Tag, Layer TargetLayer);
	GameObject* Find(const char* Tag);
	GameObject* Find(const char* Tag, Layer TargetLayer, int Index);
	void ClearAll();
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void ReleaseObjects();
	void PrepareRender(ID3D12GraphicsCommandList* CmdList);
};

// global scope framework
extern Framework framework;