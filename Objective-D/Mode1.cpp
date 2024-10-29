#include "Mode1.h"
#include "MouseUtil.h"

#include "TestObject.h"

// �ش� cpp���ϰ� h������ Ư���� ��带 �����ϰ�, �ش� ��忡 �����ϴ� ��ü���� ��Ʈ���ϱ� ���� �����̴�.
// �ݵ�� cpp, h���Ϸ� �и��Ǿ��־�� �ϸ�, �� ��忡 ���� ���ӽ����̽��� ���еǾ���Ѵ�.
// ����_���::���Ӹ��() ������ ��� �Լ� ���ӽ����̽��� ��� �̸��� �ۼ��ϴ� ���� �����Ѵ�
// �Ʒ��� ���� ������ ��� ���� �Լ� �� ��Ʈ�ѷ� �Լ��� �ۼ����ֱ⸸ �ϸ� �����ӿ�ũ�� �˾Ƽ� ���ش�.
// ���α׷� ���� �� �Ʒ��� �Լ����� [ DirectX 3D ] -> [ Main ] -> Scene.cpp�� �ִ� Init() �Լ����� ����ȴ�.
// ���� ������ Scene.cpp���� �����ϵ��� �Ѵ�.�P

// ������ ��� �ڵ� �ۼ��� ���� [ Template ] ���Ϳ� ���ø��� ����� �ξ����� ������ �� �Լ� �̸��� ���ӽ����̽� �̸��� �ٲٸ� �ȴ�.

void Mode1::Start() {
	// �ʿ��� ��ü �߰�
	scene.AddObject(new TestObject, "test_object", LAYER_1);
	
	// scene�� ��Ʈ�ѷ� ���
	RegisterController();

	// ��� �̸��� Scene�� ���
	scene.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	// ���⿡ ��� ���� �� �ʿ��� �۾� �߰� (���ҽ� �޸� ���� ��)
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc ���� �� ���α׷� ����
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// ���α׷��� �����ϴ� Scene ��� �Լ�
		scene.Exit();

	// ��ü�� Ű���� �Է�
	scene.InputKey(hWnd, nMessageID, wParam, lParam, "test_object");
}

//  ���콺 ����� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseMotionController(HWND hWnd) {
	// ���콺 ��ǥ�� ����Ʈ ��ǥ�� ��ȯ�Ѵ�.
	mouse.UpdateMousePosition(hWnd);

	// ��ü�� ���콺 ��� �Է�
	scene.InputMouseMotion(hWnd, "test_object");
}

// ���콺 ��ư Ŭ�� �̺�Ʈ�� ������ ��ü �����ͷ� �����Ѵ�
void Mode1::MouseController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// ��ü�� ���콺 �Է�
	scene.InputMouse(hWnd, nMessageID, wParam, lParam, "test_object");
}

void Mode1::RegisterController() {
	// Ű���� ��Ʈ�ѷ� �Լ� �����͸� Scene�� ���
	scene.RegisterKeyController(KeyboardController);
	// ���콺 ��Ʈ�ѷ� �Լ� �����͸� Scene�� ���
	scene.RegisterMouseController(MouseController);
	// ���콺 ��� ��Ʈ�ѷ� �����͸� Scene�� ���
	scene.RegisterMouseMotionController(MouseMotionController);
	// ��� �Ҹ��ڸ� Scene�� ���
	scene.RegisterDestructor(Destructor);
}