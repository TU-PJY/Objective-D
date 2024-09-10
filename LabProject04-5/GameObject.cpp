#include "GameObject.h"
#include "CameraUtil.h"

void GameObject::SetMesh(Mesh*& MeshPtr, std::string MeshName) {
	MeshPtr = meshUtil.GetMesh(MeshName);
	if (MeshPtr)
		MeshPtr->ReleaseUploadBuffers();
}

void GameObject::SetShader(Shader*& ShaderPtr, Shader* ShaderData) {
	ShaderPtr = ShaderData;
}

void GameObject::InitMatrix() {
	TranslateMatrix = Mat4::Identity();
	RotateMatrix = Mat4::Identity();
	ScaleMatrix = Mat4::Identity();
}

void GameObject::SetColor(XMFLOAT3 Color) {
	ModelColor = Color;
}

void GameObject::SetColor(float R, float G, float B) {
	ModelColor.x = R;
	ModelColor.y = G;
	ModelColor.z = B;
}

void GameObject::MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
}

void GameObject::MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
}

void GameObject::MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
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