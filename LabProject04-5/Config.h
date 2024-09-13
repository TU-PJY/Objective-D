#pragma once
#include <tchar.h>

// 레이어 설정
// 터레인 레이어는 반드시 있어야 한다.
// 레이어를 추가할 경우 맨 끝에 END 표시를 반드시 해줘야 한다.
enum class Layer
{ Terrain, Object, END };

// 카메라 모드 설정
// 카메라 클래스 멤버 함수 Update(float FT)에서 실행할 때 사용하는 열거형 클래스이다. 카메라 클래스에서 자세히 설명하도록 한다.
enum class CamMode
{ MODE1 };

// 프로그램 시작 시 사용할 윈도우 사이즈를 설정한다.
constexpr int START_UP_WIDTH = 1200;
constexpr int START_UP_HEIGHT = 800;

// 풀스크린 모드로 시작할 지를 설정한다.
constexpr bool START_WITH_FULL_SCREEN = false;

// 제목표시줄 문자열을 설정한다. 반드시 아래와 같은 형식으로 저정해야 한다.
constexpr TCHAR TitleName[] = _T("Objective-D(");