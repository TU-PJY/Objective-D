#include "PickingUtil.h"
#include "MouseUtil.h"

// 피킹을 담당하는 유틸이다

// 커서를 사용하여 오브젝트를 피킹한다
// 피킹할 대상의 클래스 포인터와 그 포인터가 가지는 매쉬가 필요하다
bool PickingUtil::PickByCursor(LPARAM lparam, GameObject* Object, Mesh* MeshPtr) {
	if (!MeshPtr)
		return false;

	float xclient = LOWORD(lparam);
	float yclient = HIWORD(lparam);

	XMFLOAT3 xmf3pickposition;
	xmf3pickposition.x = mouse.x;
	xmf3pickposition.y = mouse.y;
	xmf3pickposition.z = 1.0f;

	XMVECTOR xmvpickposition = XMLoadFloat3(&xmf3pickposition);
	XMMATRIX xmmtxview = XMLoadFloat4x4(&camera.ViewMatrix);

	int nintersected = 0;
	float fnearesthitdistance = FLT_MAX;
	float fhitdistance = FLT_MAX;
	nintersected = Object->PickRayInter(MeshPtr, xmvpickposition, xmmtxview, &fhitdistance);

	if ((nintersected > 0) && (fhitdistance < fnearesthitdistance))
		return true;

	return false;
}

// 원리는 위의 함수와 같으나 부포트 좌표를 사용하여 오브젝트를 피킹한다
bool PickingUtil::PickByCoord(float x, float y, GameObject* Object, Mesh* MeshPtr) {
	if (!MeshPtr)
		return false;

	float xclient = (x + 1.0) / 2.0 * SCREEN_WIDTH;
	float yclient = (1.0 - y) / 2.0 * SCREEN_HEIGHT;

	XMFLOAT3 xmf3pickposition;
	xmf3pickposition.x = (((2.0f * xclient) / (float)SCREEN_WIDTH) - 1) / camera.ProjectionMatrix._11;
	xmf3pickposition.y = -(((2.0f * yclient) / (float)SCREEN_HEIGHT) - 1) / camera.ProjectionMatrix._22;
	xmf3pickposition.z = 1.0f;

	XMVECTOR xmvpickposition = XMLoadFloat3(&xmf3pickposition);
	XMMATRIX xmmtxview = XMLoadFloat4x4(&camera.ViewMatrix);

	int nintersected = 0;
	float fnearesthitdistance = FLT_MAX;
	float fhitdistance = FLT_MAX;
	nintersected = Object->PickRayInter(MeshPtr, xmvpickposition, xmmtxview, &fhitdistance);

	if ((nintersected > 0) && (fhitdistance < fnearesthitdistance))
		return true;

	return false;
}