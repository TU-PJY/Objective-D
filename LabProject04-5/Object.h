//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once
#include "D3D_CONF.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include <string>


class Shader;


class OBJ {
public:
	// define custom member function here /////////////
	virtual void SetRotation() {}















	///////////////////////////////////////////////////

	XMFLOAT4X4 Matrix{};
	XMFLOAT3 ModelColor{};

	Shader* m_pShader{};
	Mesh* m_pMesh{};

	D3D_Layer Layer{};
	std::string Tag{};

	OBJ();
	~OBJ();

	void SetMesh(Mesh* pMesh);
	void SetShader(Shader* pShader);

	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void ReleaseShaderVariables();
	virtual void Update(float fTimeElapsed);
	virtual void OnPrepareRender();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera = NULL);

	virtual void ReleaseUploadBuffers();


	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void InitTransform();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);

	void SetColor(XMFLOAT3 xmf3Color) { ModelColor = xmf3Color; }

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3 *pxmf3Axis, float fAngle);
};