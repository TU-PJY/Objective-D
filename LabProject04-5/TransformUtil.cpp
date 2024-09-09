#include "TransformUtil.h"

void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	Matrix._41 = X;
	Matrix._42 = Y;
	Matrix._43 = Z;
}

void Transform::Rotate(XMFLOAT4X4& Matrix, float Pitch, float Yaw, float Roll) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

void Transform::Scale(XMFLOAT4X4& Matrix, float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}
