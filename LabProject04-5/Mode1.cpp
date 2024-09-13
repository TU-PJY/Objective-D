#include "Mode1.h"
#include "MouseUtil.h"

#include "Example.h"

// 해당 cpp파일과 h파일은 특정한 모드를 실행하고, 해당 모드에 존재하는 객체들을 컨트롤하기 위한 파일이다. 
// 반드시 cpp, h파일로 분리되어있어야 하며, 각 모드에 따라 네임스페이스로 구분되어야한다.
// 게임_모드::게임모드() 식으로 모드 함수 네임스페이스와 모드 이름을 작성하는 것을 권장한다
// 아래와 같은 형태의 모드 실행 함수 및 컨트롤러 함수를 작성해주기만 하면 프레임워크가 알아서 해준다.
// 프로그램 시작 시 아래의 함수들은 [ DirectX 3D ] -> [ Main ] -> DirectX_3D_Main.cpp에 있는 Init() 함수에서 실행된다. 
// 관련 내용은 DirectX_3D_Main.cpp에서 설명하도록 한다.

const char* Mode_1::Mode1() {

	// 모드 시작 시 배경색 변경
	SetBackgroundColorRGB(135, 206, 235);

	// 각각의 모드는 각각의 컨트롤러를 가짐
	// 해당 함수는 모드 시작 시 반드시 실행되어야 정확한 동작을 보장할 수 있음
	SetController();

	framework.AddObject(new Object, "object", Layer::L1);

	// 실행 함수 이름이 곧 실행하는 모드의 이름이 된다. 따라서 모드 시작 함수의 이름은 신중하게 정해야 한다.
	// 현재 함수를 실행할 경우 문자열 "Mode1"을 리턴하여 프레임워크에 전달한다
	return __func__;
}

void Mode_1::KeyboardController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// esc 누를 시 프로그램 종료
	if (nMessageID == WM_KEYDOWN && wParam == VK_ESCAPE)
		// 프로그램을 종료하는 프레임워크 멤버 함수
		framework.Exit();

}

//  마우스 모션을 지정된 객체 포인터로 전달한다
void Mode_1::MouseMotionController(HWND hWnd) {

}

// 마우스 버튼 클릭 이벤트를 지정된 객체 포인터로 전달한다
void Mode_1::MouseButtonController(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	// 좌클릭, 우클릭 눌림 상태를 업데이트 한다
	mouse.UpdateButtonState(nMessageID);

}

// 각각의 모드가 가지는 컨트롤러를 설정한다. 
// 프레임워크 멤버함수인 컨트롤러 포인터에 해당 컨트롤러 함수 포인터가 저장되어 프레임워크는 저장된 포인터의 컨트롤러만 실행한다.
void Mode_1::SetController() {
	// 키보드 컨트롤러 함수 포인터를 프레임워크에 전달한다
	framework.SetKeyController(KeyboardController);

	// 마우스 컨트롤러 함수 포인터를 프레임워크에 전달한다
	framework.SetMouseController(MouseButtonController);

	// 마우스 모션 컨트롤러 포인터를 프레임워크에 전달한다
	framework.SetMouseMotionController(MouseMotionController);
}
