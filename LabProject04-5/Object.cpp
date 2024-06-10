#include "D3D_Header.h"
#include "Object.h"


void OBJ::InitTransform() {
	Matrix = Mat4::Identity();
}


void OBJ::SetPosition(float x, float y, float z) {
	Matrix._41 = x;
	Matrix._42 = y;
	Matrix._43 = z;
}


void OBJ::SetPosition(XMFLOAT3 xmf3Position) {
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
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


void OBJ::MoveStrafe(float fDistance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Right, fDistance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::MoveUp(float fDistance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Up, fDistance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::MoveForward(float fDistance) {
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vec3::Add(xmf3Position, xmf3Look, fDistance);
	OBJ::SetPosition(xmf3Position);
}


void OBJ::Rotate(float fPitch, float fYaw, float fRoll) {
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	Matrix = Mat4::Multiply(mtxRotate, Matrix);
}


void OBJ::Rotate(XMFLOAT3* pxmf3Axis, float fAngle) {
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	Matrix = Mat4::Multiply(mtxRotate, Matrix);
}





OBJ::OBJ() {
	Matrix = Mat4::Identity();
}


OBJ::~OBJ() {
}


void OBJ::SetMesh(Mesh* pMesh) {
	m_pMesh = pMesh;
}


void OBJ::SetShader(Shader* pShader) {
	m_pShader = pShader;
}


void OBJ::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {}


void OBJ::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);

	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}


void OBJ::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera) {
	OnPrepareRender();

	if (m_pShader)
		m_pShader->Render(pd3dCommandList, pCamera);

	UpdateShaderVariables(pd3dCommandList);

	if (m_pMesh)
		m_pMesh->Render(pd3dCommandList);
}


void OBJ::ReleaseUploadBuffers() {
	if (m_pMesh)
		m_pMesh->ReleaseUploadBuffers();
}


void OBJ::ReleaseShaderVariables() {}


void OBJ::Update(float fTimeElapsed) {}


void OBJ::OnPrepareRender() {}

