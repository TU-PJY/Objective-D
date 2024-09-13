#include "TransformUtil.h"

// ��ȯ�� ����ϴ� ��ƿ�̴�
// ������ �� ���� ��Ŀ� ���� Move, Rotate, Scale�� �����ϰ� �ϵ��� �Ϸ� ������ 
// ��ȯ�� ���� Ŀ���� �� �� �ֵ��� ����� ���۷����� �����ϵ��� �ߴ�

// �����̵� ��ȯ
void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	Matrix._41 = X;
	Matrix._42 = Y;
	Matrix._43 = Z;
}

// ȸ�� ��ȯ
void Transform::Rotate(XMFLOAT4X4& Matrix, float Pitch, float Yaw, float Roll) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// ������ ��ȯ
void Transform::Scale(XMFLOAT4X4& Matrix, float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}
