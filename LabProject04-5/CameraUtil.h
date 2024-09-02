#pragma once
#include "Config.h"
#include "DirectX_3D.h"
#include "ObjectBase.h"
#define ASPECT_RATIO				(float(WIDTH) / float(HEIGHT))

class Camera {
protected:
	XMFLOAT3						Position{};

	XMFLOAT3						Right{};
	XMFLOAT3						Up{};
	XMFLOAT3						Look{};

	float           				Pitch{};
	float           				Roll{};
	float           				Yaw{};

	XMFLOAT3						LookAtWorld{};
	XMFLOAT3						Offset{};
	float           				MovingDelay{};


#ifdef _WITH_DIERECTX_MATH_FRUSTUM
	BoundingFrustum					FrustumView{};
	BoundingFrustum					FrustumWorld{};
	XMFLOAT4X4						InverseView{};
#else
	XMFLOAT4						FrustumPlane[6]{};
#endif

	D3D12_RECT						ScissorRect{};

private:
	CamMode Mode{};


public:
	D3D12_VIEWPORT					Viewport{};
	XMFLOAT4X4						ViewMatrix{};
	XMFLOAT4X4						ProjectionMatrix{};

	Camera();
	virtual ~Camera() {}

	void Update(float FT);

	virtual void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {}
	virtual void ReleaseShaderVariables() {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);

	void GenerateViewMatrix();
	void GenerateViewMatrix(XMFLOAT3 Position, XMFLOAT3 LookAt, XMFLOAT3 Up);
	void RegenerateViewMatrix();
	void GenerateProjectionMatrix(float NearPlane, float FarPlane, float AspRatio, float Fov);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float zMin = 0.0f, float zMax = 1.0f);
	void SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom);
	virtual void SetViewportsAndScissorRects(ID3D12GraphicsCommandList* CmdList);
	void SetCameraMode(CamMode Mode);

	void SetPosition(XMFLOAT3 Position);
	XMFLOAT3& GetPosition();

	void SetLookAtPosition(XMFLOAT3 LookAt);
	XMFLOAT3& GetLookAtPosition();

	XMFLOAT3& GetRightVector();
	XMFLOAT3& GetUpVector();
	XMFLOAT3& GetLookVector();

	float& GetPitch();
	float& GetRoll();
	float& GetYaw();

	void SetOffset(XMFLOAT3 Value);
	XMFLOAT3& GetOffset();

	void SetTimeLag(float DelayValue);
	float GetTimeLag();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	D3D12_VIEWPORT GetViewport();
	D3D12_RECT GetScissorRect();

	void Move(const XMFLOAT3& Shift);
	void Track(XMFLOAT3& LookAt, BASE* Object, float fTimeElapsed);
	void SetLookAt(XMFLOAT3& LookAt, BASE* Object);
	void CalculateFrustumPlanes();
	bool IsInFrustum(BoundingBox& BoundingBox);
	bool IsInFrustum(BoundingOrientedBox& BoundingBox);
};

extern Camera camera;