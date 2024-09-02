#pragma once
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include <vector>

class Shader;

typedef ID3D12GraphicsCommandList* (CommandList);

class BASE {
public:
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();

	XMFLOAT3 ModelColor{};

	XMFLOAT3 Look{ 0.0, 0.0, 1.0 };
	XMFLOAT3 Up{ 0.0, 1.0, 0.0 };
	XMFLOAT3 Right{ 1.0, 0.0, 0.0 };

	Shader* ObjectShader{};
	Mesh* ObjectMesh{};
	Mesh* TerrainMesh{};

	std::string ObjectTag{};
	bool DeleteMark{};
	bool StaticMark{};

	void SetMesh(Mesh*& MeshPtr, std::string MeshName) {
		MeshPtr = meshUtil.GetMesh(MeshName);
		if (MeshPtr)
			MeshPtr->ReleaseUploadBuffers();
	}

	void SetTerrain(Mesh*& TerrainMeshPtr, std::string TerrainMeshName) {
		TerrainMeshPtr = meshUtil.GetTerrain(TerrainMeshName);
		if (TerrainMeshPtr)
			TerrainMeshPtr->ReleaseUploadBuffers();
	}

	void SetShader(Shader*& ShaderPtr, Shader* ShaderData) {
		ShaderPtr = ShaderData;
	}

	void UpdateOOBB(BoundingOrientedBox& OOBB, Mesh*& MeshPtr) {
		if (MeshPtr) {
			XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotateMatrix), XMLoadFloat4x4(&TranslateMatrix));
			MeshPtr->OOBB.Transform(OOBB, ResultMatrix);
			XMStoreFloat4(&OOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&OOBB.Orientation)));
		}
	}

	void BeginProcess() {
		TranslateMatrix = Mat4::Identity();
		RotateMatrix = Mat4::Identity();
		SetColor(XMFLOAT3(0.0, 0.0, 0.0));
	}

	void SetPosition(float x, float y, float z) {
		TranslateMatrix._41 = x;
		TranslateMatrix._42 = y;
		TranslateMatrix._43 = z;
	}

	void SetPosition(XMFLOAT3 Position) {
		TranslateMatrix._41 = Position.x;
		TranslateMatrix._42 = Position.y;
		TranslateMatrix._43 = Position.z;
	}

	void SetColor(XMFLOAT3 Color) {
		ModelColor = Color;
	}

	void MoveStrafe(XMFLOAT3& Position, float Distance) {
		Position = Vec3::Add(Position, Right, Distance);
	}

	void MoveForward(XMFLOAT3& Position, float Distance) {
		Position = Vec3::Add(Position, Look, Distance);
	}

	void MoveUp(XMFLOAT3& Position, float Distance) {
		Position = Vec3::Add(Position, Up, Distance);
	}

	void Rotate(float Pitch, float Yaw, float Roll) {
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

	void Rotate(XMFLOAT3* Axis, float Angle) {
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(Axis), XMConvertToRadians(Angle));
		RotateMatrix = Mat4::Multiply(mtxRotate, RotateMatrix);
	}

	void LookAt(XMFLOAT3& Position, XMFLOAT3& TargetPosition, XMFLOAT3& UpVector) {
		XMFLOAT4X4 xmf4x4View = Mat4::LookAtLH(Position, TargetPosition, UpVector);
		RotateMatrix._11 = xmf4x4View._11; RotateMatrix._12 = xmf4x4View._21; RotateMatrix._13 = xmf4x4View._31;
		RotateMatrix._21 = xmf4x4View._12; RotateMatrix._22 = xmf4x4View._22; RotateMatrix._23 = xmf4x4View._32;
		RotateMatrix._31 = xmf4x4View._13; RotateMatrix._32 = xmf4x4View._23; RotateMatrix._33 = xmf4x4View._33;

		Up = Vec3::Normalize(XMFLOAT3(RotateMatrix._21, RotateMatrix._22, RotateMatrix._23));
		Right = Vec3::Normalize(XMFLOAT3(RotateMatrix._11, RotateMatrix._12, RotateMatrix._13));
		Look = Vec3::Normalize(XMFLOAT3(RotateMatrix._31, RotateMatrix._32, RotateMatrix._33));
	}

	void Scale(float ScaleX, float ScaleY, float ScaleZ) {
		XMMATRIX scaleMatrix = XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
		TranslateMatrix = Mat4::Multiply(scaleMatrix, TranslateMatrix);
	}

	void LinearAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT) {
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

	void LinearDcc(float& CurrentSpeed, float DecelerationValue, float FT) {
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

	void LerpAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT) {
		CurrentSpeed = std::lerp(CurrentSpeed, SpeedLimit, AccelerationValue * FT);
	}

	void LerpDcc(float& CurrentSpeed, float DecelerationValue, float FT) {
		CurrentSpeed = std::lerp(CurrentSpeed, 0.0, DecelerationValue * FT);
	}

	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection) {
		XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&TranslateMatrix) * xmmtxView);
		XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
		xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
		xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
		xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
	}

	int PickRayInter(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance) {
		int nIntersected = 0;

		if (ObjectMesh) {
			XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
			GenPickingRay(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
			nIntersected = ObjectMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);
		}

		return(nIntersected);
	}

	////////// virtual functions

	virtual ~BASE() {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
		XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&RotateMatrix), XMLoadFloat4x4(&TranslateMatrix));
		XMFLOAT4X4 xmf4x4World;
		XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
		CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
		CmdList->SetGraphicsRoot32BitConstants(1, 3, &ModelColor, 16);
	}

	virtual void ReleaseShaderVariables() {}

	virtual void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {}

	virtual void Update(float FT) {}

	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, Mesh* MeshPtr) {
		if (ShaderPtr)
			ShaderPtr->Render(CmdList);

			UpdateShaderVariables(CmdList);

		if (MeshPtr)
			MeshPtr->Render(CmdList);
	}

	virtual void Render(CommandList CmdList) {}
	virtual Mesh* GetTerrainMesh() { return {}; }
	virtual XMFLOAT4X4 GetTerrainMatrix() {return TranslateMatrix;}
	virtual XMFLOAT3 GetPosition(){ return {}; }
	virtual void InputNewPosition(float X=0.0, float Y=0.0, float Z=0.0) {}
	virtual BoundingOrientedBox GetOOBB() { return {}; }
};

// dummy object for avoiding iterator error
class DUMMY : public BASE {};