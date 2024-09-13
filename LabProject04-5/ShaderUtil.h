#pragma once
#include "DirectX_3D.h"

struct CB_GAMEOBJECT_INFO {
	XMFLOAT4X4						Matrix;
};

class Shader {
private:
	int								Ref = 0;

public:
	Shader() {}
	virtual ~Shader();

	void AddRef();
	void Release();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);
	D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* FileName, LPCSTR Shadername, LPCSTR ShaderProfile, ID3DBlob** ShaderBlob);
	virtual void CreateShader(ID3D12Device* Device, ID3D12RootSignature* RootSignature);
	virtual void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {}
	virtual void ReleaseShaderVariables() {}
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* CmdList);
	virtual void Render(ID3D12GraphicsCommandList* CmdList);

protected:
	ID3D12PipelineState* PipelineState = NULL;
};

class PseudoLightingShader : public Shader {
public:
	PseudoLightingShader(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
	virtual ~PseudoLightingShader() {}

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);
};
