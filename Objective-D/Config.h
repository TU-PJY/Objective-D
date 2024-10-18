#pragma once
#include <tchar.h>

// ī�޶� ��� ����
// ī�޶� Ŭ���� ��� �Լ� Update(float FT)���� ������ �� ����ϴ� ������ Ŭ�����̴�. ī�޶� Ŭ�������� �ڼ��� �����ϵ��� �Ѵ�.
enum class CamMode
{
	MODE1
};

// ���α׷� ���� �� ����� ������ ����� �����Ѵ�.
constexpr int START_UP_WIDTH = 1500;
constexpr int START_UP_HEIGHT = 800;

// Ǯ��ũ�� ���� ������ ���� �����Ѵ�.
constexpr bool START_WITH_FULL_SCREEN = false;

// �ִ� ������ ������ �����Ѵ�. 0���� ���� �� �ִ� ���������� �����Ѵ�.
constexpr float FRAME_LIMITS = 0;

// �ٿ��ڽ� ǥ�� ���θ� �����Ѵ�.
#define SHOW_BOUND_BOX

// ���� ���忡 ����� ���̾ �����Ѵ�. �� �κп��� �ݵ�� ENDǥ�⸦ �ؾ��Ѵ�.
// UI�� ��� ���� �ֻ�� ���̾ �߰��ϴ� ���� �����Ѵ�.
enum class Layer
{ L1, L2, L3, END };

// ����ǥ���� ���ڿ��� �����Ѵ�. �ݵ�� �Ʒ��� ���� �������� �����ؾ� �Ѵ�.
constexpr TCHAR TitleName[] = _T("Objective-D(");