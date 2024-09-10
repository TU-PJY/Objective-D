#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#define MAX_LOADSTRING 100
#include "Config.h"
#include "Objective-D.h"
#include "DirectX_3D_Main.h"

#include "FrameworkUtil.h"
#include "CameraUtil.h"
#include "ShaderUtil.h"
#include "MouseUtil.h"
#include "TerrainUtil.h"

int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

DirectX_3D_Main D3D_Main;

PseudoLightingShader* pseudoShader;

Framework framework;
Camera camera;
MeshUtil meshUtil;
MouseUtil mouse;
TerrainUtil terrainUtil;

HINSTANCE						AppInstance;
TCHAR							Title[MAX_LOADSTRING];
TCHAR							WindowClass[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG Messege;
	HACCEL AccelTable;

	::LoadString(hInstance, IDS_APP_TITLE, Title, MAX_LOADSTRING);
	::LoadString(hInstance, IDC_LABPROJECT045, WindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
		return(FALSE);

	AccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT045));

	while (true) {
		if (::PeekMessage(&Messege, NULL, 0, 0, PM_REMOVE)) {
			if (Messege.message == WM_QUIT)
				break;

			if (!::TranslateAccelerator(Messege.hwnd, AccelTable, &Messege)) {
				::TranslateMessage(&Messege);
				::DispatchMessage(&Messege);
			}
		}

		else
			D3D_Main.Update();
	}

	D3D_Main.Destroy();

	return((int)Messege.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX Wcex;

	Wcex.cbSize = sizeof(WNDCLASSEX);

	Wcex.style = CS_HREDRAW | CS_VREDRAW;
	Wcex.lpfnWndProc = WndProc;
	Wcex.cbClsExtra = 0;
	Wcex.cbWndExtra = 0;
	Wcex.hInstance = hInstance;
	Wcex.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABPROJECT045));
	Wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	Wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	Wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_LABPROJECT045);
	Wcex.lpszClassName = WindowClass;
	Wcex.hIconSm = ::LoadIcon(Wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return ::RegisterClassEx(&Wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	AppInstance = hInstance;
	DWORD WindowStyle{};
	RECT Rect{};

	Rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	WindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	AdjustWindowRect(&Rect, WindowStyle, FALSE);

	HWND MainWnd = CreateWindow(
		WindowClass,
		Title,
		WindowStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Rect.right - Rect.left,
		Rect.bottom - Rect.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!MainWnd)
		return(FALSE);

	D3D_Main.Create(hInstance, MainWnd);

	::ShowWindow(MainWnd, nCmdShow);
	::UpdateWindow(MainWnd);

	if (!START_WITH_FULL_SCREEN)
		D3D_Main.SwitchToWindowMode(MainWnd);

	return(TRUE);
}

void DisplayStateChanger(HWND hWnd, UINT nMessageID, WPARAM wParam) {
	switch (nMessageID) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F11:
			if (D3D_Main.FullScreenState)
				D3D_Main.SwitchToWindowMode(hWnd);
			else
				D3D_Main.SwitchToFullscreenMode(hWnd);
			break;
		}
		break;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	int wmId, wmEvent;
	PAINTSTRUCT PaintStruct;
	HDC Hdc;

	DisplayStateChanger(hWnd, nMessageID, wParam);

	switch (nMessageID) {
	case WM_SIZE:
		SCREEN_WIDTH = LOWORD(lParam);
		SCREEN_HEIGHT = HIWORD(lParam);
		D3D_Main.CLIENT_WIDTH = SCREEN_WIDTH;
		D3D_Main.CLIENT_HEIGHT = SCREEN_HEIGHT;

		camera.GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 45.0f);
		//camera.SetViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f);
		//camera.SetScissorRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		framework.InputMouseButton(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_MOUSEMOVE:
		D3D_Main.WindowsMessageFunc(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId) {
		case IDM_ABOUT:
			::DialogBox(AppInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case IDM_EXIT:
			::DestroyWindow(hWnd);
			break;

		default:
			return(::DefWindowProc(hWnd, nMessageID, wParam, lParam));
		}

		break;

	case WM_PAINT:
		Hdc = ::BeginPaint(hWnd, &PaintStruct);
		EndPaint(hWnd, &PaintStruct);
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return(::DefWindowProc(hWnd, nMessageID, wParam, lParam));
	}

	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (nMessageID) {
	case WM_INITDIALOG:
		return((INT_PTR)TRUE);

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			::EndDialog(hDlg, LOWORD(wParam));
			return((INT_PTR)TRUE);
		}
		break;
	}

	return((INT_PTR)FALSE);
}