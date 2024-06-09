#pragma once

#include "Object.h"
#include "Camera.h"


struct CB_GAMEOBJECT_INFO {
	XMFLOAT4X4						Matrix;
};




class Shader {
private:
	int								m_nReferences = 0;


public:
	Shader::Shader() {}

	virtual Shader::~Shader() {
		if (m_pd3dPipelineState) m_pd3dPipelineState->Release();
		m_pd3dPipelineState = NULL;

		ReleaseShaderVariables();
	}

	void AddRef() { 
		m_nReferences++; 
	}


	void Release() { 
		if (--m_nReferences <= 0) 
			delete this; 
	}


	virtual D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout() {
		D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
		d3dInputLayoutDesc.pInputElementDescs = NULL;
		d3dInputLayoutDesc.NumElements = 0;

		return(d3dInputLayoutDesc);
	}


	virtual D3D12_RASTERIZER_DESC Shader::CreateRasterizerState() {
		D3D12_RASTERIZER_DESC d3dRasterizerDesc;
		::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
		d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
		d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		d3dRasterizerDesc.FrontCounterClockwise = FALSE;
		d3dRasterizerDesc.DepthBias = 0;
		d3dRasterizerDesc.DepthBiasClamp = 0.0f;
		d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
		d3dRasterizerDesc.DepthClipEnable = TRUE;
		d3dRasterizerDesc.MultisampleEnable = FALSE;
		d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
		d3dRasterizerDesc.ForcedSampleCount = 0;
		d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		return(d3dRasterizerDesc);
	}


	virtual D3D12_BLEND_DESC Shader::CreateBlendState() {
		D3D12_BLEND_DESC d3dBlendDesc;
		::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
		d3dBlendDesc.AlphaToCoverageEnable = FALSE;
		d3dBlendDesc.IndependentBlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
		d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
		d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
		d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		return(d3dBlendDesc);
	}


	virtual D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState() {
		D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
		::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
		d3dDepthStencilDesc.DepthEnable = TRUE;
		d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		d3dDepthStencilDesc.StencilEnable = FALSE;
		d3dDepthStencilDesc.StencilReadMask = 0x00;
		d3dDepthStencilDesc.StencilWriteMask = 0x00;
		d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
		d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
		d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

		return(d3dDepthStencilDesc);
	}


	virtual D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob) {
		D3D12_SHADER_BYTECODE d3dShaderByteCode;
		d3dShaderByteCode.BytecodeLength = 0;
		d3dShaderByteCode.pShaderBytecode = NULL;

		return(d3dShaderByteCode);
	}


	virtual D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob) {
		D3D12_SHADER_BYTECODE d3dShaderByteCode;
		d3dShaderByteCode.BytecodeLength = 0;
		d3dShaderByteCode.pShaderBytecode = NULL;

		return(d3dShaderByteCode);
	}


	D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob) {
		UINT nCompileFlags = 0;
#if defined(_DEBUG)
		nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		::D3DCompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, NULL);

		D3D12_SHADER_BYTECODE d3dShaderByteCode;
		d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
		d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();

		return(d3dShaderByteCode);
	}


	virtual void Shader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature) {
		ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
		::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
		d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
		d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
		d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
		d3dPipelineStateDesc.BlendState = CreateBlendState();
		d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
		d3dPipelineStateDesc.InputLayout = CreateInputLayout();
		d3dPipelineStateDesc.SampleMask = UINT_MAX;
		d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		d3dPipelineStateDesc.NumRenderTargets = 1;
		d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		d3dPipelineStateDesc.SampleDesc.Count = 1;
		d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
		pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

		if (pd3dVertexShaderBlob) pd3dVertexShaderBlob->Release();
		if (pd3dPixelShaderBlob) pd3dPixelShaderBlob->Release();

		if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
	}


	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) {}
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList) {}
	virtual void ReleaseShaderVariables() {}


	virtual void Shader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList) {
		if (m_pd3dPipelineState) 
			pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
	}


	virtual void Shader::Render(ID3D12GraphicsCommandList* pd3dCommandList, Camera* pCamera) {
		OnPrepareRender(pd3dCommandList);
	}

protected:
	ID3D12PipelineState				*m_pd3dPipelineState = NULL;
};




class PseudoLightingShader : public Shader {
public:
	PseudoLightingShader() {}
	virtual ~PseudoLightingShader() {}


	virtual D3D12_INPUT_LAYOUT_DESC PseudoLightingShader::CreateInputLayout() {
		UINT nInputElementDescs = 3;
		D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

		pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		pd3dInputElementDescs[2] = { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
		d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
		d3dInputLayoutDesc.NumElements = nInputElementDescs;

		return(d3dInputLayoutDesc);
	}


	virtual D3D12_SHADER_BYTECODE PseudoLightingShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob) {
		return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSPseudoLighting", "vs_5_1", ppd3dShaderBlob));
	}


	virtual D3D12_SHADER_BYTECODE PseudoLightingShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob) {
		return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSPseudoLighting", "ps_5_1", ppd3dShaderBlob));
	}
};
