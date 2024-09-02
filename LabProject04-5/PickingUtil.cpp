#include "PickingUtil.h"

bool PickingUtil::PickByCursor(LPARAM lParam, int ObjectIntersected) {
	float xClient = LOWORD(lParam);
	float yClient = HIWORD(lParam);

	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)camera.Viewport.Width) - 1) / camera.ProjectionMatrix._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)camera.Viewport.Height) - 1) / camera.ProjectionMatrix._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&camera.ViewMatrix);

	float fNearestHitDistance = FLT_MAX;
	float fHitDistance = FLT_MAX;

	if ((ObjectIntersected > 0) && (fHitDistance < fNearestHitDistance))
		return true;

	return false;
}

bool PickingUtil::PickByCoord(float X, float Y, int ObjectIntersected) {
	float xClient = (X + 1.0) / 2.0 * WIDTH;
	float yClient = (1.0 - Y) / 2.0 * HEIGHT;

	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)camera.Viewport.Width) - 1) / camera.ProjectionMatrix._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)camera.Viewport.Height) - 1) / camera.ProjectionMatrix._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&camera.ViewMatrix);

	float fNearestHitDistance = FLT_MAX;
	float fHitDistance = FLT_MAX;

	if ((ObjectIntersected > 0) && (fHitDistance < fNearestHitDistance))
		return true;

	return false;
}