#include "Mode1.h"
#include "Ufo.h"

std::string Mode_1::Mode1() {
	// 모드에 필요한 객체 추가
	framework.AddObject(new Aircraft, "obj1", Layer::L1);
	framework.AddObject(new Object, "obj2", Layer::L1);
	framework.AddObject(new Map, "map", Layer::Terrain);

	// 각각의 모드는 각각의 컨트롤러를 가짐
	framework.SetKeyController(KeyboardController);
	framework.SetMouseController(MouseButtonController);
	framework.SetMouseMotionController(MouseMotionController);

	return __func__;
}

void Mode_1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// 원하는 객체만 컨트롤할 수 있다
	if (auto obj = framework.Find("obj1"); obj)
		obj->InputKey(hWnd, nMessageID, wParam, lParam);

	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		framework.Exit();
}

void Mode_1::MouseMotionController(HWND hWnd) {
	if (auto obj = framework.Find("obj1"); obj)
		obj->InputMouseMotion(hWnd, mouse.PrevCursorPos);
}

void Mode_1::MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	mouse.UpdateButtonState(nMessageID);

	if (auto obj = framework.Find("obj1"); obj)
		obj->InputMouseButton(hWnd, nMessageID, wParam, lParam);
}