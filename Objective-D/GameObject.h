#pragma once
#include "GameResource.h"
#include "CBVManager.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);


class GameObject {
public:
	// 각 객체는 각자의 행렬을 가진다.
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();
	XMFLOAT4X4 ScaleMatrix = Mat4::Identity();

	// 매쉬 색상
	XMFLOAT3 ModelColor{};

	// 텍스처 투명도
	float ObjectAlpha{ 1.0f };

	// 렌더링 타입, 해당 렌더링 타입에 따라 렌더링 형식이 달라진다.
	int RenderType = RENDER_TYPE_PERS;

	// 프레임워크 오브젝트 리스트에서 검색하기 위한 태그와 삭제될 오브젝트임을 알리는 삭제 마크이다.
	// 이 두 멤버변수들은 프로그래머가 직접 건들일이 없다.
	const char* ObjectTag{};
	bool DeleteMark{};

	void InitMatrix(ID3D12GraphicsCommandList* CmdList, int RenderTypeFlag=RENDER_TYPE_PERS);
	void SetColor(XMFLOAT3 Color);
	void SetColor(float R, float G, float B);
	void MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance);
	void MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance);
	void MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance);
	void BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr);
	void UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha=1.0, bool DepthTestFlag=true);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, int FlipType);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	void InputLightInfo(ID3D12GraphicsCommandList* CmdList);
	float ASP(float Value);
	void UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY);
	void UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY);
	void SetToImageMode(ID3D12GraphicsCommandList* CmdList);

	void SetToDefaultMode(ID3D12GraphicsCommandList* CmdList);

	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

	////////// virtual functions

	// 아래 함수들은 모든 객체에서 커스텀 가능한 버추얼 함수들이다. 필요하다면 새로운 버추얼 함수를 작성하여 사용할 수 있다.
	virtual ~GameObject() {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
	virtual void ReleaseShaderVariables() {}
	virtual void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseMotion(HWND hWnd, POINT MotionPosition) {}
	virtual void Update(float FT) {}
	virtual void Render(CommandList CmdList) {}
	virtual Mesh* GetObjectMesh() { return {}; }
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual OOBB GetOOBB() { return {}; }
	virtual XMFLOAT3 GetUp() { return {}; }
	virtual XMFLOAT3 GetRight() { return {}; }
	virtual XMFLOAT3 GetLook() { return {}; }
};