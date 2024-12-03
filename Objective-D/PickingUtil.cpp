#include "PickingUtil.h"
#include "MouseUtil.h"
#include "MathUtil.h"

// ��ŷ�� ����ϴ� ��ƿ�̴�

// Ŀ���� ����Ͽ� ������Ʈ�� ��ŷ�Ѵ�
// ��ŷ�� ����� ��ü �����Ϳ� �� ��ü�� ����ϴ� �Ž� �����Ͱ� �ʿ��ϴ�.
bool PickingUtil::PickByCursor(LPARAM lParam, GameObject* Object, Mesh* MeshPtr) {
	if (!MeshPtr)
		return false;
	
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * LOWORD(lParam)) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * HIWORD(lParam)) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	std::cout << PickPoint.x << " " << PickPoint.y << "\n";

	int Intersected{};
	float NearestHitDist = FLT_MAX;
	float HitDist = FLT_MAX;
	Intersected = Object->PickRayInter(MeshPtr, XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), &HitDist);

	if (Intersected > 0 && HitDist < NearestHitDist)
		return true;

	return false;
}

// ������ ��ǥ�� ����Ͽ� ������Ʈ�� ��ŷ�Ѵ�
bool PickingUtil::PickByWinCoord(int X, int Y, GameObject* Object, Mesh* MeshPtr) {
	if (!MeshPtr)
		return false;

	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * X) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * Y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	std::cout << PickPoint.x << " " << PickPoint.y << "\n";

	int Intersected{};
	float NearestHitDist = FLT_MAX;
	float HitDist = FLT_MAX;
	Intersected = Object->PickRayInter(MeshPtr, XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), &HitDist);

	if (Intersected > 0 && HitDist < NearestHitDist)
		return true;

	return false;
}