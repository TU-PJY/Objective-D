#pragma once
#include "GameResource.h"
#include "CBVUtil.h"
#include "TransformUtil.h"
#include "CollisionUtil.h"
#include "MathUtil.h"
#include "LineBrush.h"

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

	// ������Ʈ ���ο��� ����ϴ� Ŀ�ǵ� ����Ʈ
	ID3D12GraphicsCommandList* ObjectCmdList{};

	// �����ӿ�ũ ������Ʈ ����Ʈ���� �˻��ϱ� ���� �±׿� ������ ������Ʈ���� �˸��� ���� ��ũ�̴�.
	// �� �� ����������� ���α׷��Ӱ� ���� �ǵ����� ����.
	const char* ObjectTag{};
	bool DeleteMark{};

	void InitRenderState(int RenderTypeFlagg = RENDER_TYPE_PERS);
	void SetColor(XMFLOAT3 Color);
	void SetColorRGB(float R, float G, float B);
	void EnableLight();
	void FlipTexture(int FlipType);
	void DisableLight();
	void RenderMesh(Mesh* MeshPtr, Texture* TexturePtr, Shader* ShaderPtr, float Alpha=1.0f, bool DepthTestFlag=true);
	float ASP(float Value);
	void InputCommandList(ID3D12GraphicsCommandList* CmdList);
	void UseShader(Shader* ShaderPtr, bool DepthTest);
	void UseMesh(Mesh* MeshPtr);
	void UpdateShaderVariables();
	void UpdateMotionRotation(float& RotationX, float& RotationY, float DeltaX, float DeltaY);
	void UpdateMotionRotation(XMFLOAT3& Rotation, float DeltaX, float DeltaY);
	int PickRayInter(Mesh* MeshPtr, XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);
	void BindTexture(Texture* TexturePtr);

private:
	void GenPickingRay(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	void SetToImageMode();
	void SetToDefaultMode();
	void SetCamera();

	////////// virtual functions
public:
	// �Ʒ� �Լ����� ��� ��ü���� Ŀ���� ������ ���߾� �Լ����̴�. �ʿ��ϴٸ� ���ο� ���߾� �Լ��� �ۼ��Ͽ� ����� �� �ִ�.
	virtual ~GameObject() {}
	virtual void ReleaseShaderVariables() {}
	virtual void InputKey(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouse(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}
	virtual void InputMouseMotion(HWND hWnd, POINT MotionPosition) {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual Mesh* GetObjectMesh() { return {}; }
	virtual XMFLOAT3 GetPosition() { return {}; }
	virtual OOBB GetOOBB() { return {}; }
	virtual XMFLOAT3 GetUp() { return {}; }
	virtual XMFLOAT3 GetRight() { return {}; }
	virtual XMFLOAT3 GetLook() { return {}; }
	virtual Vector GetVectorSet() { return {}; }
};