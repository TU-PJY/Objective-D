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


void Framework::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		break;
	}
}


void Framework::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	switch (nMessageID) {
	case WM_KEYDOWN:
	{
		auto ufo = fw.FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
		if (ufo) ufo->ObjectController(nMessageID, wParam);
	}
	break;


	case WM_KEYUP:
	{
		auto ufo = fw.FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
		if (ufo) ufo->ObjectController(nMessageID, wParam);
	}
	break;
	}
}


void Framework::MouseMoveController(POINT PrevCursorPosition, HWND hwnd) {
	static UCHAR pKeysBuffer[256];
	float cxDelta = 0.0f, cyDelta = 0.0f;

	if (GetCapture() == hwnd) {
		::SetCursor(NULL);
		POINT CursorPos;
		::GetCursorPos(&CursorPos);
		cxDelta = (float)(CursorPos.x - PrevCursorPosition.x) / 3.0f;
		cyDelta = (float)(CursorPos.y - PrevCursorPosition.y) / 3.0f;
		::SetCursorPos(PrevCursorPosition.x, PrevCursorPosition.y);

		std::cout << cxDelta << std::endl;

		
		if (cxDelta || cyDelta) {
			auto ptr = FindObject("ufo_target", LayerRange::Single, LayerFW::L1);
			if (ptr) {
				ptr->Rotation.y += cxDelta;
				ptr->Rotation.x += cyDelta;
			}
		}
	}
}