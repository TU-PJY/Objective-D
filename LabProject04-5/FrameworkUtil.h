#pragma once
#include "Config.h"
#include "GameObject.h"
#include "CameraUtil.h"
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include "ResourceList.h"
#include <deque>
#include <ranges>
#include <map>

// global scope shader
constexpr int NUM_LAYER = static_cast<int>(Layer::END);

enum class ButtonState
{
	Down, Up
};

enum class  ButtonType
{
	LButton, RButton
};

class Framework {
private:
	std::multimap<std::string, GameObject*> ObjectList;

	std::string RunningMode{};
	void (*MouseController)(HWND, UINT, WPARAM, LPARAM);
	void (*MouseMotionController)(HWND);
	void (*KeyboardController)(HWND, UINT, WPARAM, LPARAM);

protected:
	ID3D12RootSignature* RootSignature{};
	std::array<std::deque<GameObject*>, NUM_LAYER> Container;

public:
	std::string Mode();
	POINT PrevCursorPos{};
	bool LB_DownState{}, RB_DownState{};

	void Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, std::string ModeFunction(),
		void KeyboardControllerPtr(HWND, UINT, WPARAM, LPARAM) = nullptr,
		void MouseControllerPtr(HWND, UINT, WPARAM, LPARAM) = nullptr,
		void MouseMotionController(HWND) = nullptr);

	void SwitchMode(std::string ModeFunction(),
		void KeyboardControllerPtr(HWND, UINT, WPARAM, LPARAM) = nullptr,
		void MouseControllerPtr(HWND, UINT, WPARAM, LPARAM) = nullptr,
		void MouseMotionControllerPtr(HWND) = nullptr);

	void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void InputMouseMotion(HWND hWnd);
	void CaptureMouseMotion(HWND hWnd);
	POINT NewCursorPos();
	void ReleaseMouseMotion();
	void UpdateMouseButton(ButtonType Button, ButtonState ButtonState);
	void Exit();
	void Update(float FT);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void UpdateContainer(int Index);
	void AddObject(GameObject*&& Object, std::string Tag, Layer Layer);
	void DeleteObject(GameObject* Object);
	void DeleteObject(std::string Tag);
	void DeleteObject(std::string Tag, Layer TargetLayer);
	GameObject* Find(std::string Tag);
	GameObject* Find(std::string Tag, Layer TargetLayer, int Index);
	Mesh* FindMesh(std::string MeshName);
	Mesh* FindTerrain(std::string TerrainMeshName);
	void ClearAll();
	bool CheckCollision(const BoundingOrientedBox& OOBBFrom, const BoundingOrientedBox& OOBBTo);
	Mesh* MeshLoader(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode = true);
	bool CheckTerrainFloor(XMFLOAT3 Position, GameObject* Terrain);
	void ClampToTerrainFloor(XMFLOAT3& Position, GameObject* Terrain);
	void CheckCollisionTerrain(XMFLOAT3& Position, GameObject* Terrain);
	PseudoLightingShader* LoadShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* Device);
	ID3D12RootSignature* GetGraphicsRootSignature();
	void ReleaseObjects();
	void ReleaseUploadBuffers();
	void PrepareRender(ID3D12GraphicsCommandList* CmdList);
};

// global scope framework
extern Framework framework;