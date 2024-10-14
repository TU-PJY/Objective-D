#include "TransformUtil.h"

// 변환을 담당하는 유틸이다
// 원래는 세 가지 행렬에 각각 Move, Rotate, Scale만 가능하게 하도록 하려 했으나 
// 변환을 직접 커스텀 할 수 있도록 행렬을 레퍼런스로 전달하도록 했다

// 평행이동 변환
void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	XMMATRIX TranslateMat = XMMatrixTranslation(X, Y, Z);
	Matrix = Mat4::Multiply(TranslateMat, Matrix);
}

// 회전 변환
void Transform::Rotate(XMFLOAT4X4& Matrix, float Pitch, float Yaw, float Roll) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// 스케일 변환
void Transform::Scale(XMFLOAT4X4& Matrix, float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}
