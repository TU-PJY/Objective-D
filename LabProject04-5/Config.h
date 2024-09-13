#pragma once
#include <tchar.h>

// ���̾� ����
// �ͷ��� ���̾�� �ݵ�� �־�� �Ѵ�.
// ���̾ �߰��� ��� �� ���� END ǥ�ø� �ݵ�� ����� �Ѵ�.
enum class Layer
{ Terrain, Object, END };

// ī�޶� ��� ����
// ī�޶� Ŭ���� ��� �Լ� Update(float FT)���� ������ �� ����ϴ� ������ Ŭ�����̴�. ī�޶� Ŭ�������� �ڼ��� �����ϵ��� �Ѵ�.
enum class CamMode
{ MODE1 };

// ���α׷� ���� �� ����� ������ ����� �����Ѵ�.
constexpr int START_UP_WIDTH = 1200;
constexpr int START_UP_HEIGHT = 800;

// Ǯ��ũ�� ���� ������ ���� �����Ѵ�.
constexpr bool START_WITH_FULL_SCREEN = false;

// ����ǥ���� ���ڿ��� �����Ѵ�. �ݵ�� �Ʒ��� ���� �������� �����ؾ� �Ѵ�.
constexpr TCHAR TitleName[] = _T("Objective-D(");