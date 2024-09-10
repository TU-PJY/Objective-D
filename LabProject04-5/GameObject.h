#pragma once
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

typedef struct Vector {
	XMFLOAT3 Up;
	XMFLOAT3 Right;
	XMFLOAT3 Look;
}ObjectVector;

class GameObject {
public:
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();
	XMFLOAT4X4 ScaleMatrix = Mat4::Identity();

	XMFLOAT3 ModelColor{};

	std::string ObjectTag{};
	bool DeleteMark{};

	void SetMesh(Mesh*& MeshPtr, std::string MeshName);
	void SetShader(Shader*& ShaderPtr, Shader* ShaderData);
	void InitMatrix();
	void SetColor(XMFLOAT3 Color);
	void SetColor(float R, float G, float B);
	void MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance);
	void MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance);
	void MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance);
	//void Rotate(XMFLOAT3* Axis, float Angle);
	void LinearAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT);
	void LinearDcc(float& CurrentSpeed, float DecelerationValue, float FT);
	void LerpAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT);
	void LerpDcc(float& CurrentSpeed, float DecelerationValue, float FT);
	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, Mesh* MeshPtr);

	////////// virtual functions

	virtual ~GameObject() {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
	virtual void ReleaseShaderVariables() {}
	virtual void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseButton(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseMotion(HWND hWnd, POINT PrevCursorPos) {}
	virtual void Update(float FT) {}
	virtual void Render(CommandList CmdList) {}
	virtual Mesh* GetObjectMesh() { return {}; }
	virtual Mesh* GetTerrainMesh() { return {}; }
	virtual XMFLOAT4X4 GetTerrainMatrix() { return TranslateMatrix; }
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual void InputNewPosition(float X = 0.0, float Y = 0.0, float Z = 0.0) {}
	virtual OOBB GetOOBB() { return {}; }
	virtual XMFLOAT3 GetUp() { return {}; }
	virtual XMFLOAT3 GetRight() { return {}; }
	virtual XMFLOAT3 GetLook() { return {}; }
};

// dummy object for avoiding iterator error
class DUMMY : public GameObject {};