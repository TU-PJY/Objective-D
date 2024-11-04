#pragma once
#include "GameResource.h"
#include "CBVUtil.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"

class Shader;
typedef ID3D12GraphicsCommandList* (CommandList);


class GameObject {
public:
	// �� ��ü�� ������ ����� ������.
	XMFLOAT4X4 TranslateMatrix = Mat4::Identity();
	XMFLOAT4X4 RotateMatrix = Mat4::Identity();
	XMFLOAT4X4 ScaleMatrix = Mat4::Identity();

	// �Ž� ����
	XMFLOAT3 ModelColor{};

	// �ؽ�ó ����
	float ObjectAlpha{ 1.0f };

	// ������ Ÿ��, �ش� ������ Ÿ�Կ� ���� ������ ������ �޶�����.
	int RenderType = RENDER_TYPE_PERS;

	// �����ӿ�ũ ������Ʈ ����Ʈ���� �˻��ϱ� ���� �±׿� ������ ������Ʈ���� �˸��� ���� ��ũ�̴�.
	// �� �� ����������� ���α׷��Ӱ� ���� �ǵ����� ����.
	const char* ObjectTag{};
	bool DeleteMark{};

	void InitRenderState(ID3D12GraphicsCommandList* CmdList, int RenderTypeFlag=RENDER_TYPE_PERS);
	void SetColor(XMFLOAT3 Color);
	void SetColorRGB(float R, float G, float B);
	void RenderMesh(ID3D12GraphicsCommandList* CmdList, Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha=1.0, bool DepthTestFlag=true);
	void FlipTexture(ID3D12GraphicsCommandList* CmdList, int FlipType);
	void DisableLight(ID3D12GraphicsCommandList* CmdList);
	void EnableLight(ID3D12GraphicsCommandList* CmdList);
	float ASP(float Value);
	void UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY);
	void UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

private:
	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	void BindTexture(ID3D12GraphicsCommandList* CmdList, Texture* TexturePtr);
	void UseShader(ID3D12GraphicsCommandList* CmdList, Shader* ShaderPtr, bool DepthTest);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
	void SetToImageMode(ID3D12GraphicsCommandList* CmdList);
	void SetToDefaultMode(ID3D12GraphicsCommandList* CmdList);
	void SetCamera(ID3D12GraphicsCommandList* CmdList);

	////////// virtual functions
public:
	// �Ʒ� �Լ����� ��� ��ü���� Ŀ���� ������ ���߾� �Լ����̴�. �ʿ��ϴٸ� ���ο� ���߾� �Լ��� �ۼ��Ͽ� ����� �� �ִ�.
	virtual ~GameObject() {}
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
	virtual Vector GetVectorSet() { return {}; }
};