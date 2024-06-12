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
	return(XMFLOAT3(Matrix._41, Matrix._42, Matrix._43));
}


XMFLOAT3 OBJ::GetLook() {
	return(Vec3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33)));
}


XMFLOAT3 OBJ::GetUp() {
	return(Vec3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23)));
}


XMFLOAT3 OBJ::GetRight() {
	return(Vec3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13)));
}


void OBJ::MoveStrafe(float Distance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Right, Distance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::MoveUp(float Distance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Up, Distance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::MoveForward(float Distance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Look, Distance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::Rotate(float Pitch, float Yaw, float Roll) {
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	Matrix = Mat4::Multiply(mtxRotate, Matrix);
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

