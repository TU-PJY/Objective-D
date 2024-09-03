#include "PickingUtil.h"
#include "GameObject.h"

bool PickingUtil::PickByCursor(LPARAM lparam, GameObject* Object, Mesh* MeshPtr) {
	float xclient = LOWORD(lparam);
	float yclient = HIWORD(lparam);

	XMFLOAT3 xmf3pickposition;
	xmf3pickposition.x = (((2.0f * xclient) / (float)camera.Viewport.Width) - 1) / camera.ProjectionMatrix._11;
	xmf3pickposition.y = -(((2.0f * yclient) / (float)camera.Viewport.Height) - 1) / camera.ProjectionMatrix._22;
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

bool PickingUtil::PickByCoord(float x, float y, GameObject* Object, Mesh* MeshPtr) {
	float xclient = (x + 1.0) / 2.0 * WIDTH;
	float yclient = (1.0 - y) / 2.0 * HEIGHT;

	XMFLOAT3 xmf3pickposition;
	xmf3pickposition.x = (((2.0f * xclient) / (float)camera.Viewport.Width) - 1) / camera.ProjectionMatrix._11;
	xmf3pickposition.y = -(((2.0f * yclient) / (float)camera.Viewport.Height) - 1) / camera.ProjectionMatrix._22;
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