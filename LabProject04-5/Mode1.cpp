#include "Mode1.h"
#include "MouseUtil.h"

#include "TestObject.h"

// 해당 cpp파일과 h파일은 특정한 모드를 실행하고, 해당 모드에 존재하는 객체들을 컨트롤하기 위한 파일이다. 
// 반드시 cpp, h파일로 분리되어있어야 하며, 각 모드에 따라 네임스페이스로 구분되어야한다.
// 게임_모드::게임모드() 식으로 모드 함수 네임스페이스와 모드 이름을 작성하는 것을 권장한다
// 아래와 같은 형태의 모드 실행 함수 및 컨트롤러 함수를 작성해주기만 하면 프레임워크가 알아서 해준다.
// 프로그램 시작 시 아래의 함수들은 [ DirectX 3D ] -> [ Main ] -> DirectX_3D_Main.cpp에 있는 Init() 함수에서 실행된다. 
// 관련 내용은 DirectX_3D_Main.cpp에서 설명하도록 한다.

// 간편한 모드 코드 작성을 위해 [ Template ] 필터에 템플릿을 만들어 두었으니 복붙한 후 함수 이름과 네임스페이스 이름을 바꾸면 된다.

void Mode1::Start() {
	// 모드 시작 시 배경색 변경
	SetBackgroundColorRGB(135, 206, 235);

	// 각각의 모드는 각각의 컨트롤러를 가짐
	// 해당 함수는 모드 시작 시 반드시 실행되어야 정확한 동작을 보장할 수 있음
	framework.AddObject(new TestObject, "test_object");

	// 여기에 모드 시작 시 필요한 작업 추가 (객체 추가 등)
	
	// 키보드 컨트롤러 함수 포인터를 프레임워크에 등록
	framework.RegisterKeyController(KeyboardController);
	// 마우스 컨트롤러 함수 포인터를 프레임워크에 등록
	framework.RegisterMouseController(MouseButtonController);
	// 마우스 모션 컨트롤러 포인터를 프레임워크에 등록
	framework.RegisterMouseMotionController(MouseMotionController);
	// 모드 소멸자를 프레임워크에 등록
	framework.RegisterDestructor(Destructor);

	// 모드 이름을 프레임워크에 등록
	framework.RegisterModeName("Mode1");
}

void Mode1::Destructor() {
	// 여기에 모드 종료 시 필요한 작업 추가 (리소스 메모리 해제 등)
}

void Mode1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// 프로그램을 종료하는 프레임워크 멤버 함수
		framework.Exit();

	if (auto object = framework.Find("test_object"); object)
		object->InputKey(hWnd, nMessageID, wParam, lParam);
}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void Mode1::MouseMotionController(HWND hWnd) {
	if (auto object = framework.Find("test_object"); object)
		object->InputMouseMotion(hWnd, mouse.PrevCursorPos);
}

// 마우스 버튼 클릭 이벤트를 지정된 객체 포인터로 전달한다
void Mode1::MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// 좌클릭, 우클릭 눌림 상태를 업데이트 한다
	mouse.UpdateButtonState(nMessageID);

	if (auto object = framework.Find("test_object"); object)
		object->InputMouseButton(hWnd, nMessageID, wParam, lParam);
}