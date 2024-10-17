#include "CollisionUtil.h"

// 충돌 처리를 담당하는 유틸이다.
// 서로 다른 종류의 바운딩 객체와도 비교 가능하며, 객체가 가지는 위치, 회전, 크기를 파라미터에 넣어주면 된다.

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
	oobb.Extents = Size;
	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z);
	XMStoreFloat4(&oobb.Orientation, quaternion);
}

bool OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const BoundingBox& Other) {
	if (oobb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const BoundingSphere& Other) {
	if (oobb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void AABB::Update(const XMFLOAT3& Center, const XMFLOAT3& Size) {
	aabb.Center = Center;
	aabb.Extents = Size;
}

bool AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const BoundingOrientedBox& Other) {
	if (aabb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const BoundingSphere& Other) {
	if (aabb.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

void Range::Update(const XMFLOAT3& Center, const float& Size) {
	sphere.Center = Center;
	sphere.Radius = Size;
}

bool Range::CheckCollision(const Range& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool Range::CheckCollision(const BoundingBox& Other) {
	if (sphere.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool Range::CheckCollision(const BoundingOrientedBox& Other) {
	if (sphere.Intersects(Other)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}