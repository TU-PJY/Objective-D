#pragma once
#include "D3DHeader.h"


struct CB_GAMEOBJECT_INFO {
	XMFLOAT4X4						Matrix;
};



class Shader {
private:
	int								Ref = 0;


public:
	Shader();

	virtual ~Shader();


	void AddRef() { 
		Ref++; 
	}


	void Release() { 
		if (--Ref <= 0) 
			delete this; 
	}


	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();

	virtual D3D12_BLEND_DESC CreateBlendState();

	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);

	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR* FileName, LPCSTR ShaderName, LPCSTR ShaderProfile, ID3DBlob** ShaderBlob);

	virtual void CreateShader(ID3D12Device* Device, ID3D12RootSignature* RootSignature);

	virtual void CreateShaderVariables(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList);
	virtual void ReleaseShaderVariables();

	virtual void OnPrepareRender(ID3D12GraphicsCommandList* CmdList);

	virtual void Render(ID3D12GraphicsCommandList* CmdList);

protected:
	ID3D12PipelineState				*PipelineState = NULL;
};




class PseudoLightingShader : public Shader {
public:
	PseudoLightingShader();

	virtual ~PseudoLightingShader();

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();

	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);

	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);
};
