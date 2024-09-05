#include "Mode1.h"
#include "Ufo.h"

std::string Mode_1::Mode1() {
	// ��忡 �ʿ��� ��ü �߰�
	framework.AddObject(new Aircraft, "obj1", Layer::L1);
	framework.AddObject(new Object, "obj2", Layer::L1);
	framework.AddObject(new Map, "map", Layer::Terrain);

	// ������ ���� ������ ��Ʈ�ѷ��� ����
	framework.SetKeyController(KeyboardController);
	framework.SetMouseController(MouseButtonController);
	framework.SetMouseMotionController(MouseMotionController);

	return __func__;
}

void Mode_1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ���ϴ� ��ü�� ��Ʈ���� �� �ִ�
	if (auto obj = framework.Find("obj1"); obj)
		obj->InputKey(hWnd, nMessageID, wParam, lParam);

	// esc ���� �� ���α׷� ����
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