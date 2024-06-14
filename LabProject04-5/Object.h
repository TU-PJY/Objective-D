//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once
#include "D3DHeader.h"
#include "CONF.h"
#include "Shader.h"
#include "Mesh.h"

class Shader;


class OBJ {
public:
	// define custom member function here /////////////
	




	///////////////////////////////////////////////////
	XMFLOAT4X4 Matrix{};
	XMFLOAT3 ModelColor{};

	XMFLOAT3 Position{};
	XMFLOAT3 Rotation{};

	XMFLOAT3 Look{0.0, 0.0, 1.0};
	XMFLOAT3 Up{0.0, 1.0, 0.0};
	XMFLOAT3 Right{1.0, 0.0, 0.0};

	Shader* ObjectShader{};
	Mesh* ObjectMesh{};

	LayerFW Layer{};
	std::string Tag{};


	OBJ() {
		Matrix = Mat4::Identity();
	}

	~OBJ() {
	}

	void SetMesh(Mesh* MeshData) {
		ObjectMesh = MeshData;
	}

	void SetShader(Shader* ShaderData) {
		ObjectShader = ShaderData;
	}

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&Matrix)));
		CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);

		CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
	}

	virtual void Render(ID3D12GraphicsCommandList* CmdList) {
		OnPrepareRender();

		if (ObjectShader)
			ObjectShader->Render(CmdList);

		UpdateShaderVariables(CmdList);

		if (ObjectMesh)
			ObjectMesh->Render(CmdList);
	}

	virtual void ReleaseUploadBuffers() {
		if (ObjectMesh)
			ObjectMesh->ReleaseUploadBuffers();
	}

	virtual void ReleaseShaderVariables() {}

	virtual void Update(float FT) {}

	virtual void OnPrepareRender() {}

	virtual void ObjectController(UINT nMessageID, WPARAM wParam) {}


	void InitTransform() {
		Matrix = Mat4::Identity();
	}

	void SetPosition(float x, float y, float z) {
		Matrix._41 = x;
		Matrix._42 = y;
		Matrix._43 = z;
	}

	void SetPosition(XMFLOAT3 Position) {
		Matrix._41 = Position.x;
		Matrix._42 = Position.y;
		Matrix._43 = Position.z;
	}

	void SetColor(XMFLOAT3 Color) { 
		ModelColor = Color; 
	}

	void MoveStrafe(float Distance) {
		Position = Vec3::Add(Position, Right, Distance);
		OBJ::SetPosition(Position);
	}


	void MoveForward(float Distance) {
		Position = Vec3::Add(Position, Look, Distance);
		OBJ::SetPosition(Position);
	}


	void MoveUp(float Distance) {
		Position = Vec3::Add(Position, Up, Distance);
		OBJ::SetPosition(Position);
	}

	void Rotate(float Pitch, float Yaw, float Roll) {
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


	void Rotate(XMFLOAT3* Axis, float Angle) {
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(Axis), XMConvertToRadians(Angle));
		Matrix = Mat4::Multiply(mtxRotate, Matrix);
	}
};