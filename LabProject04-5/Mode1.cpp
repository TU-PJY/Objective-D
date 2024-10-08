#include "Mode1.h"
#include "MouseUtil.h"

#include "TestObject.h"

// �ش� cpp���ϰ� h������ Ư���� ��带 �����ϰ�, �ش� ��忡 �����ϴ� ��ü���� ��Ʈ���ϱ� ���� �����̴�. 
// �ݵ�� cpp, h���Ϸ� �и��Ǿ��־�� �ϸ�, �� ��忡 ���� ���ӽ����̽��� ���еǾ���Ѵ�.
// ����_���::���Ӹ��() ������ ��� �Լ� ���ӽ����̽��� ��� �̸��� �ۼ��ϴ� ���� �����Ѵ�
// �Ʒ��� ���� ������ ��� ���� �Լ� �� ��Ʈ�ѷ� �Լ��� �ۼ����ֱ⸸ �ϸ� �����ӿ�ũ�� �˾Ƽ� ���ش�.
// ���α׷� ���� �� �Ʒ��� �Լ����� [ DirectX 3D ] -> [ Main ] -> DirectX_3D_Main.cpp�� �ִ� Init() �Լ����� ����ȴ�. 
// ���� ������ DirectX_3D_Main.cpp���� �����ϵ��� �Ѵ�.

// ������ ��� �ڵ� �ۼ��� ���� [ Template ] ���Ϳ� ���ø��� ����� �ξ����� ������ �� �Լ� �̸��� ���ӽ����̽� �̸��� �ٲٸ� �ȴ�.

void Mode1::Start() {
	// ��� ���� �� ���� ����
	SetBackgroundColorRGB(135, 206, 235);

	// ������ ���� ������ ��Ʈ�ѷ��� ����
	// �ش� �Լ��� ��� ���� �� �ݵ�� ����Ǿ�� ��Ȯ�� ������ ������ �� ����
	framework.AddObject(new TestObject, "test_object");

	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (��ü �߰� ��)
	
	// Ű���� ��Ʈ�ѷ� �Լ� �����͸� �����ӿ�ũ�� ���
	framework.RegisterKeyController(KeyboardController);
	// ���콺 ��Ʈ�ѷ� �Լ� �����͸� �����ӿ�ũ�� ���
	framework.RegisterMouseController(MouseButtonController);
	// ���콺 ��� ��Ʈ�ѷ� �����͸� �����ӿ�ũ�� ���
	framework.RegisterMouseMotionController(MouseMotionController);
	// ��� �Ҹ��ڸ� �����ӿ�ũ�� ���
	framework.RegisterDestructor(Destructor);

	// ��� �̸��� �����ӿ�ũ�� ���
	framework.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// ���α׷��� �����ϴ� �����ӿ�ũ ��� �Լ�
		framework.Exit();

}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseMotionController(HWND hWnd) {
	if (auto object = framework.Find("test_object"); object)
		object->InputMouseMotion(hWnd, mouse.PrevCursorPos);
}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ��Ŭ��, ��Ŭ�� ���� ���¸� ������Ʈ �Ѵ�
	mouse.UpdateButtonState(nMessageID);

	if (auto object = framework.Find("test_object"); object)
		object->InputMouseButton(hWnd, nMessageID, wParam, lParam);
}