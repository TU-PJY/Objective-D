#include "Object.h"


void OBJ::InitTransform() {
	Matrix = Mat4::Identity();
}


void OBJ::SetPosition(float x, float y, float z) {
	Matrix._41 = x;
	Matrix._42 = y;
	Matrix._43 = z;
}


void OBJ::SetPosition(XMFLOAT3 Position) {
	SetPosition(Position.x, Position.y, Position.z);
}


XMFLOAT3 OBJ::GetPosition() {
	return Position;
}


XMFLOAT3 OBJ::GetLook() {
	return Look;
}


XMFLOAT3 OBJ::GetUp() {
	return Up;
}


XMFLOAT3 OBJ::GetRight() {
	return Right;
}


void OBJ::MoveStrafe(float Distance) {
	Position = Vec3::Add(Position, Right, Distance);
	OBJ::SetPosition(Position);
}


void OBJ::MoveForward(float Distance) {
	Position = Vec3::Add(Position, Look, Distance);
	OBJ::SetPosition(Position);
}


void OBJ::MoveUp(float Distance) {
	Position = Vec3::Add(Position, Up, Distance);
	OBJ::SetPosition(Position);
}


void OBJ::Rotate(float Pitch, float Yaw, float Roll) {
	XMVECTOR UpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR LookVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR RightVector = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMMATRIX RotateMat = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(RotateMat, Matrix);

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


void OBJ::Rotate(XMFLOAT3* Axis, float Angle) {
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(Axis), XMConvertToRadians(Angle));
	Matrix = Mat4::Multiply(mtxRotate, Matrix);
}





OBJ::OBJ() {
	Matrix = Mat4::Identity();
}


OBJ::~OBJ() {
}


void OBJ::SetMesh(Mesh* MeshData) {
	ObjectMesh = MeshData;
}


void OBJ::SetShader(Shader* ShaderData) {
	ObjectShader = ShaderData;
}


void OBJ::CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {}


void OBJ::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix)));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);

	CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}


void OBJ::Render(ID3D12GraphicsCommandList* CmdList) {
	OnPrepareRender();

	if (ObjectShader)
		ObjectShader->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (ObjectMesh)
		ObjectMesh->Render(CmdList);
}


void OBJ::ReleaseUploadBuffers() {
	if (ObjectMesh)
		ObjectMesh->ReleaseUploadBuffers();
}


void OBJ::ReleaseShaderVariables() {}


void OBJ::Update(float FT) {}


void OBJ::OnPrepareRender() {}

