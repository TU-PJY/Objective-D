//-----------------------------------------------------------------------------
// File: CGameObject.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Object.h"
#include "Shader.h"

MAIN_OBJ::MAIN_OBJ()
{
	Matrix = Matrix4x4::Identity();
}

MAIN_OBJ::~MAIN_OBJ()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader) m_pShader->Release();
}

void MAIN_OBJ::SetMesh(Mesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

void MAIN_OBJ::SetShader(Shader *pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}


void MAIN_OBJ::Update(float fTimeElapsed){}

void MAIN_OBJ::Render(ID3D12GraphicsCommandList *pd3dCommandList, Camera *pCamera){
	OnPrepareRender();

	if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);

	UpdateShaderVariables(pd3dCommandList);

	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}

void MAIN_OBJ::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList){}

void MAIN_OBJ::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix)));
	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);

	pd3dCommandList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
}

void MAIN_OBJ::ReleaseShaderVariables()
{
}


void MAIN_OBJ::ReleaseUploadBuffers(){
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}


void MAIN_OBJ::InitTransform() {
	Matrix = Matrix4x4::Identity();
}

void MAIN_OBJ::SetPosition(float x, float y, float z)
{
	Matrix._41 = x;
	Matrix._42 = y;
	Matrix._43 = z;
}

void MAIN_OBJ::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

XMFLOAT3 MAIN_OBJ::GetPosition()
{
	return(XMFLOAT3(Matrix._41, Matrix._42, Matrix._43));
}

XMFLOAT3 MAIN_OBJ::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(Matrix._31, Matrix._32, Matrix._33)));
}

XMFLOAT3 MAIN_OBJ::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(Matrix._21, Matrix._22, Matrix._23)));
}

XMFLOAT3 MAIN_OBJ::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(Matrix._11, Matrix._12, Matrix._13)));
}

void MAIN_OBJ::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	MAIN_OBJ::SetPosition(xmf3Position);
}

void MAIN_OBJ::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	MAIN_OBJ::SetPosition(xmf3Position);
}

void MAIN_OBJ::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	MAIN_OBJ::SetPosition(xmf3Position);
}

void MAIN_OBJ::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	Matrix = Matrix4x4::Multiply(mtxRotate, Matrix);
}

void MAIN_OBJ::Rotate(XMFLOAT3 *pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	Matrix = Matrix4x4::Multiply(mtxRotate, Matrix);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


