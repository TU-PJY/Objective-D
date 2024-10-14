#pragma once
#include "ResourceManager.h"
#include "CBVManager.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

enum class RenderType
{ Pers, Ortho };

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
	const char* ObjectTag{};
	bool DeleteMark{};

	void InitMatrix(ID3D12GraphicsCommandList* CmdList, RenderType Type);
	void SetColor(XMFLOAT3 Color);
	void SetColor(float R, float G, float B);
	void MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance);
	void MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance);
	void MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance);
	void BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr);
	void UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest=true);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	void SendLightInfo(ID3D12GraphicsCommandList* CmdList);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, HeapAndBuffer& HAB_Struct, bool H_Flip, bool V_Flip, int BufferIndex);
	void SetToImageMode(ID3D12GraphicsCommandList* CmdList);
	void SetAlpha(ID3D12GraphicsCommandList* CmdList, HeapAndBuffer& HAB_Struct, float AlphaValue, int BufferIndex);

	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

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
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual OOBB GetOOBB() { return {}; }
	virtual XMFLOAT3 GetUp() { return {}; }
	virtual XMFLOAT3 GetRight() { return {}; }
	virtual XMFLOAT3 GetLook() { return {}; }
};