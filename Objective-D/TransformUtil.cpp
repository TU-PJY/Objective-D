#include "TransformUtil.h"

// 변환을 담당하는 유틸이다
// 원래는 세 가지 행렬에 각각 Move, Rotate, Scale만 가능하게 하도록 하려 했으나 
// 변환을 직접 커스텀 할 수 있도록 행렬을 레퍼런스로 전달하도록 했다

// 평행이동 변환
void Transform::Move(XMFLOAT4X4& Matrix, float X, float Y, float Z) {
	XMMATRIX TranslateMat = XMMatrixTranslation(X, Y, Z);
	Matrix = Mat4::Multiply(TranslateMat, Matrix);
}

void Transform::Rotate(XMFLOAT4X4& Matrix, float Pitch, float Yaw, float Roll) {
	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(RotateMat, Matrix);
}

// Y툭 회전 후 X축 회전 변환
void Transform::RotateYX(XMFLOAT4X4& Matrix, float Pitch, float Yaw) {
	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(-Yaw));
	XMMATRIX rotationX = XMMatrixRotationX(XMConvertToRadians(-Pitch));

	XMMATRIX rotationMat = rotationY * rotationX;

	XMMATRIX currentMat = XMLoadFloat4x4(&Matrix);
	XMMATRIX finalMat = rotationMat * currentMat;

	// 결과를 XMFLOAT4X4로 다시 저장
	XMStoreFloat4x4(&Matrix, finalMat);
}

// 스케일 변환
void Transform::Scale(XMFLOAT4X4& Matrix, float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}

// 이미지 출력 시 매쉬의 종횡비와 이미지 종횡비를 동일하게 맞춘다.
// 변환할 행렬과 이미지의 크기를 파라미터에 전달하면 된다.
void Transform::ImageAspect(XMFLOAT4X4& Matrix, int Width, int Height) {
	float ScaleX{}, ScaleY{};

	if (Width > Height) {
		ScaleX = 1.0;
		ScaleY = (float)Height / (float)Width;
	}

	else if (Width < Height) {
		ScaleX = (float)Width / (float)Height;
		ScaleY = 1.0;
	}

	else {
		ScaleX = 1.0;
		ScaleY = 1.0;
	}

	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, 1.0);
	Matrix = Mat4::Multiply(ScaleMat, Matrix);
}
