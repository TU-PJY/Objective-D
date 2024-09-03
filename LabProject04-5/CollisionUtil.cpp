#include "CollisionUtil.h"

void OOBB::Update(Mesh* MeshPtr, XMFLOAT4X4& TMatrix, XMFLOAT4X4& RMatrix) {
	if (MeshPtr) {
		XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RMatrix), XMLoadFloat4x4(&TMatrix));
		MeshPtr->OOBB.Transform(oobb, ResultMatrix);
		XMStoreFloat4(&oobb.Orientation, XMQuaternionNormalize(XMLoadFloat4(&oobb.Orientation)));
	}
}

bool OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb))
		return true;
	else
		return false;
}