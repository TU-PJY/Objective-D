#include "MathUtil.h"

// 벡터 연산을 위한 함수들이다. 객체가 가지는 벡터 삼형제와 회전값을 파라미터에 넣어주면 된다.

// 오브젝트가 회전한 후의 벡터를 업데이트 한다. 카메라 추적을 실행하기 위해서는 반드시 이 함수로 벡터를 업데이트 해야한다.
void Math::UpdateVector(XMFLOAT3& Up, XMFLOAT3& Right, XMFLOAT3& Look, float Pitch, float Yaw, float Roll) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));

	Up = {
		XMVectorGetX(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(UpVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(UpVector, RotateMat))
	};

	Look = {
		XMVectorGetX(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(LookVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(LookVector, RotateMat))
	};

	Right = {
		XMVectorGetX(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetY(XMVector3TransformNormal(RightVector, RotateMat)),
		XMVectorGetZ(XMVector3TransformNormal(RightVector, RotateMat))
	};
}

// 벡터를 초기화 한다. 벡터를 사용해야하는 객체의 경우 반드시 이 함수를 최소 한 번 실행해야한다. 
void Math::InitVector(XMFLOAT3& Up, XMFLOAT3& Right, XMFLOAT3& Look) {
	Up = XMFLOAT3(0.0, 1.0, 0.0);
	Right = XMFLOAT3(1.0, 0.0, 0.0);
	Look = XMFLOAT3(0.0, 0.0, 1.0);
}

// 벡터를 사용하여 특정 위치를 바라보게 하도록 설정한다. 자신의 행렬, 벡터, 위치와 상대의 위치, 업벡터를 파라미터에 넣으면 된다.
void Math::LookAt(XMFLOAT4X4& Matrix, XMFLOAT3& Up, XMFLOAT3& Look, XMFLOAT3& Right, XMFLOAT3& ThisPosition, XMFLOAT3& TargetPosition, XMFLOAT3& TargetUpVector) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(ThisPosition, TargetPosition, TargetUpVector);
	Matrix._11 = xmf4x4View._11; Matrix._12 = xmf4x4View._21; Matrix._13 = xmf4x4View._31;
	Matrix._21 = xmf4x4View._12; Matrix._22 = xmf4x4View._22; Matrix._23 = xmf4x4View._32;
	Matrix._31 = xmf4x4View._13; Matrix._32 = xmf4x4View._23; Matrix._33 = xmf4x4View._33;

	Up = Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23));
	Right = Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13));
	Look = Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33));
}