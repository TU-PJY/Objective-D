#include "Framework.h"
#include "ModeHeader.h"

// global scope shader
PseudoLightingShader* pShader;
// global scope camera
Camera cam;
// global scope framework
Framework fw;




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
		case '1':
		{
			auto obj = fw.FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
			if (obj) obj->SetRotation();
		}
			break;

		case '2':
		{
			auto obj = fw.FindObject("aircraft_target", LayerRange::Single, LayerFW::L1);
			if (obj)obj->SetRotation();
		}
			break;

		case VK_UP:
			cam.MoveFront = true;
			break;

		case VK_DOWN:
			cam.MoveBack = true;
			break;

		case VK_RIGHT:
			cam.MoveRight = true;
			break;

		case VK_LEFT:
			cam.MoveLeft = true;
			break;
		}
		break;


	case WM_KEYUP:
		switch (wParam) {
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;

		case VK_UP:
			cam.MoveFront = false;
			break;

		case VK_DOWN:
			cam.MoveBack = false;
			break;

		case VK_RIGHT:
			cam.MoveRight = false;
			break;

		case VK_LEFT:
			cam.MoveLeft = false;
			break;
		}
		break;
	}
}