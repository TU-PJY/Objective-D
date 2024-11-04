#include "MathUtil.h"
#include <cmath>

// 벡터 연산을 위한 함수들이다. 객체가 가지는 벡터 삼형제와 회전값을 파라미터에 넣어주면 된다.

// 벡터를 기본 값으로 초기화 한다.
void Math::InitVector(ObjectVector& VectorStruct) {
	VectorStruct.Up = XMFLOAT3(0.0, 1.0, 0.0);
	VectorStruct.Right = XMFLOAT3(1.0, 0.0, 0.0);
	VectorStruct.Look = XMFLOAT3(0.0, 0.0, 1.0);
}

// 오브젝트가 회전한 후의 벡터를 업데이트 한다. 카메라 추적을 실행하기 위해서는 반드시 이 함수로 벡터를 업데이트 해야한다.
void Math::UpdateVector(ObjectVector& VectorStruct, float Pitch, float Yaw, float Roll) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));

	VectorStruct.Up = {
		XMVectorGetX(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(UpVector, RotateMat))
	};

	VectorStruct.Look = {
		XMVectorGetX(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(LookVector, RotateMat))
	};

	VectorStruct.Right = {
		XMVectorGetX(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(RightVector, RotateMat))
	};
}

void Math::UpdateVector(ObjectVector& VectorStruct, XMFLOAT3& Rotation) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

	VectorStruct.Up = {
		XMVectorGetX(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(UpVector, RotateMat))
	};

	VectorStruct.Look = {
		XMVectorGetX(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(LookVector, RotateMat))
	};

	VectorStruct.Right = {
		XMVectorGetX(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(RightVector, RotateMat))
	};
}

// 벡터를 사용하여 특정 위치를 바라보게 하도록 설정한다. 자신의 행렬, 벡터, 위치와 상대의 위치, 업벡터를 파라미터에 넣으면 된다.
void Math::LookAt(XMFLOAT4X4& Matrix, ObjectVector& VectorStruct, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& TargetUpVector) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(TargetPosition, ThisPosition, TargetUpVector);
	Matrix._11 = xmf4x4View._11; Matrix._12 = xmf4x4View._21; Matrix._13 = xmf4x4View._31;
	Matrix._21 = xmf4x4View._12; Matrix._22 = xmf4x4View._22; Matrix._23 = xmf4x4View._32;
	Matrix._31 = xmf4x4View._13; Matrix._32 = xmf4x4View._23; Matrix._33 = xmf4x4View._33;

	VectorStruct.Up = Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23));
	VectorStruct.Right = Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13));
	VectorStruct.Look = Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33));
}

// 위치를 앞으로 움직인다. 현재 자신의 위치값과 자신의 look벡터, 속도값을 넣어주면 된다.
void Math::Vector_MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

// 위치를 옆으로 움직인다. 현재 자신의 위치값과 자신의 right벡터, 속도값을 넣어주면 된다.
void Math::Vector_MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

// 위치를 위로 움직인다. 현재 자신의 위치값과 자신의 up벡터, 속도값을 넣어주면 된다.
void Math::Vector_MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

// 현재 시점에서 앞으로 움직인다.
void Math::MoveForward(XMFLOAT3& Position, float RotationY, float MoveDistance) {
	Position.x += sin(RotationY) * MoveDistance;
	Position.z += cos(RotationY) * MoveDistance;
}

// 현재 시점에서 옆으로 움직인다.
void Math::MoveStrafe(XMFLOAT3& Position, float RotationY, float MoveDistance) {
	Position.x += cos(RotationY) * MoveDistance;
	Position.z -= sin(RotationY) * MoveDistance;
}

// 현재 시점에서 위로 움직인다.
void Math::MoveUp(XMFLOAT3& Position, float MoveDistance) {
	Position.y += MoveDistance;
}

// 2차원 거리를 계산한다.
float Math::CalcDistance2D(float FromX, float FromY, float ToX, float ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

// 2차원 각도를 계산한다.
float Math::CalcDegree2D(float FromX, float FromY, float ToX, float ToY) {
	return XMConvertToDegrees(atan2(ToY - FromY, ToX - FromX));
}

// 2차원 라디안을 계산한다.
float Math::CalcRadians2D(float FromX, float FromY, float ToX, float ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}