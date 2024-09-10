#pragma once
#include "ShaderUtil.h"
#include "MeshUtil.h"
#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

// 각 객체에 선언 가능한 벡터 삼형제 구조체
typedef struct Vector {
	XMFLOAT3 Up;
	XMFLOAT3 Right;
	XMFLOAT3 Look;
}ObjectVector;

class GameObject {
public:
	// 각 객체는 각자의 행렬을 가진다. 
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();
	XMFLOAT4X4 ScaleMatrix = Mat4::Identity();

	XMFLOAT3 ModelColor{};

	// 프레임워크 오브젝트 리스트에서 검색하기 위한 태그와 삭제될 오브젝트임을 알리는 삭제 마크이다.
	// 이 두 멤버변수들은 프로그래머가 직접 건들일이 없다.
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

	/*void LinearAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT);
	void LinearDcc(float& CurrentSpeed, float DecelerationValue, float FT);
	void LerpAcc(float& CurrentSpeed, float SpeedLimit, float AccelerationValue, float FT);
	void LerpDcc(float& CurrentSpeed, float DecelerationValue, float FT);*/

	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, Mesh* MeshPtr);

	////////// virtual functions

	// 아래 함수들은 모든 객체에서 커스텀 가능한 버추얼 함수들이다. 필요하다면 새로운 버추얼 함수를 작성하여 사용할 수 있다.
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


// 반복자가 end를 가리켜 반복자 무효화가 발생하는 경우를 방지하기 위한 더미 오브젝트이다. 
// 프레임워크 오브젝트 리스트에서 검색되지 않으며, 어떠한 업데이트도, 렌더링도 되지 않는다. 
// 프레임워크 초기화 시 각 레이어의 가장 앞부분에 추가된다.
class DUMMY : public GameObject {};