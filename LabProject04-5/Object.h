//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once
#include "D3DHeader.h"
#include "CONF.h"
#include "Shader.h"
#include "Mesh.h"

class Shader;


class OBJ {
public:
	// define custom member function here /////////////
	














	///////////////////////////////////////////////////

	XMFLOAT4X4 Matrix{};
	XMFLOAT3 ModelColor{};

	XMFLOAT3 Position{};

	XMFLOAT3 Look{0.0, 0.0, 1.0};
	XMFLOAT3 Up{0.0, 1.0, 0.0};
	XMFLOAT3 Right{1.0, 0.0, 0.0};

	Shader* ObjectShader{};
	Mesh* ObjectMesh{};

	XMFLOAT3 Velocity{};
	float MaxVelocityXZ{};
	float MaxVelocityY{};

	LayerFW Layer{};
	std::string Tag{};

	OBJ();
	~OBJ();

	void SetMesh(Mesh* MeshData);
	void SetShader(Shader* ShaderData);

	virtual void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);

	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);

	virtual void ReleaseShaderVariables();
	virtual void Update(float FT);
	virtual void OnPrepareRender();

	virtual void Render(ID3D12GraphicsCommandList* CmdList);

	virtual void ObjectController(UINT nMessageID, WPARAM wParam) {}

	virtual void ReleaseUploadBuffers();


	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	void InitTransform();

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 Position);

	void SetColor(XMFLOAT3 Color) { ModelColor = Color; }

	void MoveStrafe(float Distance = 1.0f);
	void MoveUp(float Distance = 1.0f);
	void MoveForward(float Distance = 1.0f);

	void SetVelocity(const XMFLOAT3& xmf3Shift);
	void Move(const XMFLOAT3& xmf3Shift);

	void Rotate(float Pitch = 10.0f, float Yaw = 10.0f, float Roll = 10.0f);
	void Rotate(XMFLOAT3 *Axis, float Angle);
};