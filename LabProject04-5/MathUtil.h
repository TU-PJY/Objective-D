#pragma once
#include "DirectX_3D.h"

namespace Math {
	void CalculateVec(XMFLOAT3& Up, XMFLOAT3& Look, XMFLOAT3& Right, float Pitch, float Yaw, float Roll);
	void LookAt(XMFLOAT4X4& Matrix, XMFLOAT3& Up, XMFLOAT3& Look, XMFLOAT3& Right, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& UpVector);
}