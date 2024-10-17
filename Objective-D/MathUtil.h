#pragma once
#include "DirectX_3D.h"

namespace Math {
	void UpdateVector(XMFLOAT3& Up, XMFLOAT3& Right, XMFLOAT3& Look, float Pitch, float Yaw, float Roll);
	void InitVector(XMFLOAT3& Up, XMFLOAT3& Right, XMFLOAT3& Look);
	void LookAt(XMFLOAT4X4& Matrix, XMFLOAT3& Up, XMFLOAT3& Look, XMFLOAT3& Right, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& TargetUpVector);
	float CalcDistance2D(float FromX, float FromY, float ToX, float ToY);
	float CalcDegree2D(float FromX, float FromY, float ToX, float ToY);
	float CalcRadians2D(float FromX, float FromY, float ToX, float ToY);
}