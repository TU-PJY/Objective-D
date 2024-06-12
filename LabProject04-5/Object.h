//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once
#include "D3D_Header.h"
#include "D3D_CONF.h"
#include "Shader.h"
#include "Mesh.h"

class Shader;


class OBJ {
public:
	// define custom member function here /////////////
	virtual void SetRotation() {}















	///////////////////////////////////////////////////

	XMFLOAT4X4 Matrix{};
	XMFLOAT3 ModelColor{};

	Shader* ObjectShader{};
	Mesh* ObjectMesh{};

	D3D_Layer Layer{};
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

	void Rotate(float Pitch = 10.0f, float Yaw = 10.0f, float Roll = 10.0f);
	void Rotate(XMFLOAT3 *Axis, float Angle);
};