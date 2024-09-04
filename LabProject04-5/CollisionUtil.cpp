#include "CollisionUtil.h"

void OOBB::Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix, XMFLOAT4X4& SMatrix) {
	if (MeshPtr) {
		XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMatrix), XMLoadFloat4x4(&RMatrix));
		ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMatrix));
		MeshPtr->OOBB.Transform(oobb, ResultMatrix);
		XMStoreFloat4(&oobb.Orientation, XMQuaternionNormalize(XMLoadFloat4(&oobb.Orientation)));
	}
}

void OOBB::Update(const XMFLOAT3& Center, const XMFLOAT3& Size, const XMFLOAT3& Rotation) {
	oobb.Center = Center;

	// 반직경(크기) 설정
	oobb.Extents = Size;

	// 회전 설정 (Euler 각도를 쿼터니언으로 변환)
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
	XMStoreFloat4(&oobb.Orientation, quaternion);
}

bool OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb))
		return true;
	else
		return false;
}