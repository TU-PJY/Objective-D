#include "GameObject.h"

void GameObject::SetMesh(Mesh*& MeshPtr, std::string MeshName) {
	MeshPtr = meshUtil.GetMesh(MeshName);
	if (MeshPtr)
		MeshPtr->ReleaseUploadBuffers();
}

void GameObject::SetShader(Shader*& ShaderPtr, Shader* ShaderData) {
	ShaderPtr = ShaderData;
}

void GameObject::BeginProcess() {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
}

void GameObject::SetPosition(float x, float y, float z) {
	TranslateMatrix._41 = x;
	TranslateMatrix._42 = y;
	TranslateMatrix._43 = z;
}

void GameObject::SetPosition(XMFLOAT3 Position) {
	TranslateMatrix._41 = Position.x;
	TranslateMatrix._42 = Position.y;
	TranslateMatrix._43 = Position.z;
}

void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

void GameObject::MoveStrafe(XMFLOAT3& Position, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

void GameObject::MoveForward(XMFLOAT3& Position, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

void GameObject::MoveUp(XMFLOAT3& Position, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
}

void GameObject::Rotate(float Pitch, float Yaw, float Roll) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(Pitch),
		XMConvertToRadians(Yaw),
		XMConvertToRadians(Roll)
	);

	RotateMatrix = Mat4::Multiply(RotateMat, RotateMatrix);

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

void GameObject::Rotate(XMFLOAT3* Axis, float Angle) {
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(Axis), XMConvertToRadians(Angle));
	RotateMatrix = Mat4::Multiply(mtxRotate, RotateMatrix);
}

void GameObject::LookAt(XMFLOAT3& Position, XMFLOAT3& TargetPosition, XMFLOAT3& UpVector) {
	XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(Position, TargetPosition, UpVector);
	RotateMatrix._11 = xmf4x4View._11; RotateMatrix._12 = xmf4x4View._21; RotateMatrix._13 = xmf4x4View._31;
	RotateMatrix._21 = xmf4x4View._12; RotateMatrix._22 = xmf4x4View._22; RotateMatrix._23 = xmf4x4View._32;
	RotateMatrix._31 = xmf4x4View._13; RotateMatrix._32 = xmf4x4View._23; RotateMatrix._33 = xmf4x4View._33;

	Up = Vec3::Normalize(XMFLOAT3(RotateMatrix._21, RotateMatrix._22, RotateMatrix._23));
	Right = Vec3::Normalize(XMFLOAT3(RotateMatrix._11, RotateMatrix._12, RotateMatrix._13));
	Look = Vec3::Normalize(XMFLOAT3(RotateMatrix._31, RotateMatrix._32, RotateMatrix._33));
}

void GameObject::Scale(float ScaleX, float ScaleY, float ScaleZ) {
	XMMATRIX ScaleMat = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
	TranslateMatrix = Mat4::Multiply(ScaleMat, ScaleMatrix);
}

void GameObject::LinearAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT) {
	if (SpeedLimit > 0.0) {
		CurrentSpeed += AccelerationValue * FT;
		if (CurrentSpeed >= SpeedLimit)
			CurrentSpeed = SpeedLimit;
	}

	else if (SpeedLimit < 0.0) {
		CurrentSpeed -= AccelerationValue * FT;
		if (CurrentSpeed <= SpeedLimit)
			CurrentSpeed = SpeedLimit;
	}
}

void GameObject::LinearDcc(float& CurrentSpeed, float DecelerationValue, float FT) {
	if (CurrentSpeed > 0.0) {
		CurrentSpeed -= DecelerationValue * FT;
		if (CurrentSpeed <= 0.0)
			CurrentSpeed = 0.0;
	}

	else if (CurrentSpeed < 0.0) {
		CurrentSpeed += DecelerationValue * FT;
		if (CurrentSpeed >= 0.0)
			CurrentSpeed = 0.0;
	}
}

void GameObject::LerpAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT) {
	CurrentSpeed = std::lerp(CurrentSpeed, SpeedLimit, AccelerationValue * FT);
}

void GameObject::LerpDcc(float& CurrentSpeed, float DecelerationValue, float FT) {
	CurrentSpeed = std::lerp(CurrentSpeed, 0.0, DecelerationValue * FT);
}

void GameObject::GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));

	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, ResultMatrix * xmmtxView);
	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

int GameObject::PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
	int nIntersected = 0;

	XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
	GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
	nIntersected = MeshPtr->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);

	return(nIntersected);
}

void GameObject::RenderMesh(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, Mesh* MeshPtr) {
	if (ShaderPtr)
		ShaderPtr->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (MeshPtr)
		MeshPtr->Render(CmdList);
}

////////// virtual functions

void GameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&ScaleMatrix), XMLoadFloat4x4(&RotateMatrix));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TranslateMatrix));
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}