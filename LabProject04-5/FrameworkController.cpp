#include "Framework.h"
#include "ModeHeader.h"

// global scope shader
PseudoLightingShader* pShader;
// global scope framework
Framework fw;
// global scope camera
Camera cam;


// mesh list to load
std::unordered_map<std::string, char*> MeshList {
	{ "pUfoMesh", "Models//UFO.txt" },
	{ "pFlyerMesh", "Models//FlyerPlayerShip.txt" },
};


void Framework::Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	RootSignature = CreateGraphicsRootSignature(Device);
	pShader = ShaderLoader(RootSignature, Device, CmdList);
	LoadMeshFromList(Device, CmdList);

	SetMode(Mode1);
}


void Framework::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_KEYDOWN: case WM_KEYUP:
	{
		auto obj = fw.FindObject("obj1", LayerRange::Single, Layer::L1);
		if (obj) obj->ObjectKeyboardController(nMessageID, wParam);
	}
	break;
	}
}


void Framework::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&PrevCursorPosition);
		LButtonDownState = true;
		break;

	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&PrevCursorPosition);
		RButtonDownState = true;
		break;

	case WM_LBUTTONUP:
		::ReleaseCapture();
		LButtonDownState = false;
		break;

	case WM_RBUTTONUP:
		::ReleaseCapture();
		RButtonDownState = false;
		break;
	}
}


void Framework::MouseMotionController(HWND hwnd) {
	if (GetCapture() == hwnd) {
		auto obj = FindObject("obj1", LayerRange::Single, Layer::L1);
		if (obj) obj->ObjectMouseMotionController(PrevCursorPosition, LButtonDownState, RButtonDownState);
	}
}