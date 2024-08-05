#include "FrameworkUtil.h"
#include "ModeHeader.h"
#include "Controller.h"

// global scope shader
PseudoLightingShader* pShader;
// global scope framework
Framework fw;
// global scope camera
Camera cam;

void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	RootSignature = CreateGraphicsRootSignature(Device);
	pShader = ShaderLoader(RootSignature, Device, CmdList);
	LoadMeshFromList(Device, CmdList);
	LoadTerrainFromList(Device, CmdList);

	// add dummy object
	for (int i = 0; i < NUM_LAYER; ++i)
		AddObject(new DUMMY, "__DUMMY__", static_cast<Layer>(i));

	SwitchMode(Mode1);
}

void Framework::InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	if (RunningMode == "Mode1")
		Mode_1::KeyboardController(hWnd, nMessageID, wParam, lParam);
}

void Framework::InputMouseMotion(HWND hwnd) {
	if (RunningMode == "Mode1")
		Mode_1::MouseMotionController(hwnd);
}

void Framework::InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&fw.PrevCursorPosition);
		fw.LButtonDownState = true;
		break;

	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&fw.PrevCursorPosition);
		fw.RButtonDownState = true;
		break;

	case WM_LBUTTONUP:
		::ReleaseCapture();
		fw.LButtonDownState = false;
		break;

	case WM_RBUTTONUP:
		::ReleaseCapture();
		fw.RButtonDownState = false;
		break;
	}

	if (RunningMode == "Mode1")
		Mode_1::MouseController(hWnd, nMessageID, wParam, lParam);
}