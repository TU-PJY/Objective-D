#pragma once
#include "DirectX_3D.h"

namespace Math {
	void UpdateVector(ObjectVector& VectorStruct, float Pitch, float Yaw, float Roll);
	void UpdateVector(ObjectVector& VectorStruct, XMFLOAT3& Rotation);
	void InitVector(ObjectVector& VectorStruct);
	void LookAt(XMFLOAT4X4& Matrix, ObjectVector& VectorStruct, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& TargetUpVector);
	float CalcDistance2D(float FromX, float FromY, float ToX, float ToY);
	float CalcDegree2D(float FromX, float FromY, float ToX, float ToY);
	float CalcRadians2D(float FromX, float FromY, float ToX, float ToY);
}