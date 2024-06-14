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


void Framework::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, POINT PrevCursorPos) {
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&PrevCursorPos);
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;

	case WM_MOUSEMOVE:
		break;
	}
}


void Framework::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_UP: case VK_DOWN: case VK_RIGHT: case VK_LEFT:
		{
			auto ufo = fw.FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
			if (ufo) ufo->ObjectController(nMessageID, wParam);
		}
		break;

		}
		break;


	case WM_KEYUP:
		switch (wParam) {
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;

		case VK_UP: case VK_DOWN: case VK_RIGHT: case VK_LEFT:
		{
			auto ufo = fw.FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
			if (ufo) ufo->ObjectController(nMessageID, wParam);
		}
		break;

		}
		break;
	}
}