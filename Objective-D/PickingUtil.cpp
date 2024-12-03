#include "PickingUtil.h"
#include "MouseUtil.h"
#include "MathUtil.h"

// ��ŷ�� ����ϴ� ��ƿ�̴�.

void GenBoundboxPickingRay(XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, XMVECTOR& PickRayOrigin, XMVECTOR& PickRayDirection);

// Ŀ���� ����Ͽ� �Ž��� ��ŷ�Ѵ�
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

// ������ ��ǥ�� ����Ͽ� �Ž��� ��ŷ�Ѵ�
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


// ���콺�� �ٿ�� �ڽ��� ��ŷ�Ѵ�.
bool PickingUtil::PickByCursorAABB(LPARAM lParam, const AABB& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * LOWORD(lParam)) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * HIWORD(lParam)) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}

bool PickingUtil::PickByCursorOOBB(LPARAM lParam, const OOBB& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * LOWORD(lParam)) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * HIWORD(lParam)) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}

bool PickingUtil::PickByCursorRange(LPARAM lParam, const Range& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * LOWORD(lParam)) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * HIWORD(lParam)) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}


// ������ ��ǥ�� ����Ͽ� �ٿ��ڽ��� ��ŷ�Ѵ�.
bool PickingUtil::PickByWinCoordAABB(int X, int Y, const AABB& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * X) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * Y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}

bool PickingUtil::PickByWinCoordOOBB(int X, int Y, const OOBB& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * X) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * Y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}

bool PickingUtil::PickByWinCoordRange(int X, int Y, const Range& Other) {
	XMFLOAT3 PickPoint{};
	PickPoint.x = (((2.0f * X) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	PickPoint.y = -(((2.0f * Y) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	PickPoint.z = 1.0f;

	XMVECTOR Origin, Direction;
	GenBoundboxPickingRay(XMLoadFloat3(&PickPoint), XMLoadFloat4x4(&camera.ViewMatrix), Origin, Direction);

	return Math::CheckRayCollision(Origin, Direction, Other);
}


// ������ ��ǥ�� ���� ���� ���ͷ� ��ȯ�Ѵ�. ���α׷��Ӱ� ���� ����� ���� ����.
void GenBoundboxPickingRay(XMVECTOR& PickPosition, XMMATRIX& ViewMatrix, XMVECTOR& PickRayOrigin, XMVECTOR& PickRayDirection) {
	XMMATRIX MatrixTomodel = XMMatrixInverse(NULL, ViewMatrix);
	XMFLOAT3 CameraOrigin(0.0f, 0.0f, 0.0f);
	PickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&CameraOrigin), MatrixTomodel);
	PickRayDirection = XMVector3TransformCoord(PickPosition, MatrixTomodel);
	PickRayDirection = XMVector3Normalize(PickRayDirection - PickRayOrigin);
}