#pragma once
#include "ResourceManager.h"
#include "CBVManager.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);

// �� ��ü�� ���� ������ ���� ������ ����ü
typedef struct Vector {
	XMFLOAT3 Up;
	XMFLOAT3 Right;
	XMFLOAT3 Look;
}ObjectVector;

class GameObject {
public:
	// �� ��ü�� ������ ����� ������.
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();
	XMFLOAT4X4 ScaleMatrix = Mat4::Identity();

	// �Ž� ����
	XMFLOAT3 ModelColor{};

	// �ؽ�ó ����
	float AlphaValue{ 1.0f };

	// ������ Ÿ��, �ش� ������ Ÿ�Կ� ���� ������ ������ �޶�����.
	RenderType renderType = RenderType::Pers;

	// �����ӿ�ũ ������Ʈ ����Ʈ���� �˻��ϱ� ���� �±׿� ������ ������Ʈ���� �˸��� ���� ��ũ�̴�.
	// �� �� ����������� ���α׷��Ӱ� ���� �ǵ����� ����.
	const char* ObjectTag{};
	bool DeleteMark{};

	void InitMatrix(ID3D12GraphicsCommandList* CmdList, RenderType Type=RenderType::Pers);
	void SetColor(XMFLOAT3 Color);
	void SetColor(float R, float G, float B);
	void MoveStrafe(XMFLOAT3& Position, XMFLOAT3 Right, float Distance);
	void MoveForward(XMFLOAT3& Position, XMFLOAT3 Look, float Distance);
	void MoveUp(XMFLOAT3& Position, XMFLOAT3 Up, float Distance);
	void BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr);
	void UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest = true);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	void InputLightInfo(ID3D12GraphicsCommandList* CmdList);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, bool H_Flip, bool V_Flip);
	void SetToImageMode(ID3D12GraphicsCommandList* CmdList);
	void SetAlpha(ID3D12GraphicsCommandList* CmdList, float AlphaValue);

	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

	////////// virtual functions

	// �Ʒ� �Լ����� ��� ��ü���� Ŀ���� ������ ���߾� �Լ����̴�. �ʿ��ϴٸ� ���ο� ���߾� �Լ��� �ۼ��Ͽ� ����� �� �ִ�.
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