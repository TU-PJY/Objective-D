#include "Mode1.h"
#include "MouseUtil.h"

// �ش� cpp���ϰ� h������ Ư���� ��带 �����ϰ�, �ش� ��忡 �����ϴ� ��ü���� ��Ʈ���ϱ� ���� �����̴�. 
// �ݵ�� cpp, h���Ϸ� �и��Ǿ��־�� �ϸ�, �� ��忡 ���� ���ӽ����̽��� ���еǾ���Ѵ�.
// ����_���::���Ӹ��() ������ ��� �Լ� ���ӽ����̽��� ��� �̸��� �ۼ��ϴ� ���� �����Ѵ�
// �Ʒ��� ���� ������ ��� ���� �Լ� �� ��Ʈ�ѷ� �Լ��� �ۼ����ֱ⸸ �ϸ� �����ӿ�ũ�� �˾Ƽ� ���ش�.
// ���α׷� ���� �� �Ʒ��� �Լ����� [ DirectX 3D ] -> [ Main ] -> DirectX_3D_Main.cpp�� �ִ� Init() �Լ����� ����ȴ�. 
// ���� ������ DirectX_3D_Main.cpp���� �����ϵ��� �Ѵ�.

std::string Mode_1::Mode1() {

	// ��� ���� �� ���� ����
	SetBackgroundColorRGB(135, 206, 235);

	// ������ ���� ������ ��Ʈ�ѷ��� ����
	// �ش� �Լ��� ��� ���� �� �ݵ�� ����Ǿ�� ��Ȯ�� ������ ������ �� ����
	SetController();

	// ���� �Լ� �̸��� �� �����ϴ� ����� �̸��� �ȴ�. ���� ��� ���� �Լ��� �̸��� �����ϰ� ���ؾ� �Ѵ�.
	// ���� �Լ��� ������ ��� ���ڿ� "Mode1"�� �����Ͽ� �����ӿ�ũ�� �����Ѵ�
	return __func__;
}

void Mode_1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// ���α׷��� �����ϴ� �����ӿ�ũ ��� �Լ�
		framework.Exit();

}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void Mode_1::MouseMotionController(HWND hWnd) {

}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void Mode_1::MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ��Ŭ��, ��Ŭ�� ���� ���¸� ������Ʈ �Ѵ�
	mouse.UpdateButtonState(nMessageID);

}

// ������ ��尡 ������ ��Ʈ�ѷ��� �����Ѵ�. 
// �����ӿ�ũ ����Լ��� ��Ʈ�ѷ� �����Ϳ� �ش� ��Ʈ�ѷ� �Լ� �����Ͱ� ����Ǿ� �����ӿ�ũ�� ����� �������� ��Ʈ�ѷ��� �����Ѵ�.
void Mode_1::SetController() {
	// Ű���� ��Ʈ�ѷ� �Լ� �����͸� �����ӿ�ũ�� �����Ѵ�
	framework.SetKeyController(KeyboardController);

	// ���콺 ��Ʈ�ѷ� �Լ� �����͸� �����ӿ�ũ�� �����Ѵ�
	framework.SetMouseController(MouseButtonController);

	// ���콺 ��� ��Ʈ�ѷ� �����͸� �����ӿ�ũ�� �����Ѵ�
	framework.SetMouseMotionController(MouseMotionController);
}
