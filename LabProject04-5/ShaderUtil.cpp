#include "ShaderUtil.h"

D3D12_INPUT_LAYOUT_DESC PseudoLightingShader::CreateInputLayout() {
	UINT NumInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* InputElementDescs = new D3D12_INPUT_ELEMENT_DESC[NumInputElementDescs];

	InputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[2] = { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
	InputLayoutDesc.pInputElementDescs = InputElementDescs;
	InputLayoutDesc.NumElements = NumInputElementDescs;

	return(InputLayoutDesc);
}

D3D12_SHADER_BYTECODE PseudoLightingShader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "VSPseudoLighting", "vs_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE PseudoLightingShader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Shaders.hlsl", "PSPseudoLighting", "ps_5_1", ShaderBlob));
}

//////////////////////////////////

Shader::~Shader() {
	if (PipelineState)
		PipelineState->Release();
	PipelineState = NULL;

	ReleaseShaderVariables();
}

void Shader::AddRef() {
	Ref++;
}

void Shader::Release() {
	if (--Ref <= 0)
		delete this;
}

D3D12_INPUT_LAYOUT_DESC Shader::CreateInputLayout() {
	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
	InputLayoutDesc.pInputElementDescs = NULL;
	InputLayoutDesc.NumElements = 0;

	return(InputLayoutDesc);
}

D3D12_RASTERIZER_DESC Shader::CreateRasterizerState() {
	D3D12_RASTERIZER_DESC RasterizerDesc;
	::ZeroMemory(&RasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	return(RasterizerDesc);
}

D3D12_BLEND_DESC Shader::CreateBlendState() {
	D3D12_BLEND_DESC BlendDesc;
	::ZeroMemory(&BlendDesc, sizeof(D3D12_BLEND_DESC));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = FALSE;
	BlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(BlendDesc);
}

D3D12_DEPTH_STENCIL_DESC Shader::CreateDepthStencilState() {
	D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;
	::ZeroMemory(&DepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	DepthStencilDesc.DepthEnable = TRUE;
	DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	DepthStencilDesc.StencilEnable = FALSE;
	DepthStencilDesc.StencilReadMask = 0x00;
	DepthStencilDesc.StencilWriteMask = 0x00;
	DepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	DepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;

	return(DepthStencilDesc);
}

D3D12_SHADER_BYTECODE Shader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = NULL;

	return(ShaderByteCode);
}

D3D12_SHADER_BYTECODE Shader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = 0;
	ShaderByteCode.pShaderBytecode = NULL;

	return(ShaderByteCode);
}

D3D12_SHADER_BYTECODE Shader::CompileShaderFromFile(WCHAR* FileName, LPCSTR Shadername, LPCSTR ShaderProfile, ID3DBlob** ShaderBlob) {
	UINT CompileFlags = 0;
#if defined(_DEBUG)
	CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	::D3DCompileFromFile(FileName, NULL, NULL, Shadername, ShaderProfile, CompileFlags, 0, ShaderBlob, NULL);

	D3D12_SHADER_BYTECODE ShaderByteCode;
	ShaderByteCode.BytecodeLength = (*ShaderBlob)->GetBufferSize();
	ShaderByteCode.pShaderBytecode = (*ShaderBlob)->GetBufferPointer();

	return(ShaderByteCode);
}

void Shader::CreateShader(ID3D12Device* Device, ID3D12RootSignature* RootSignature) {
	ID3DBlob* VertexShaderBlob = NULL, * PixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc;
	::ZeroMemory(&PipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	PipelineStateDesc.pRootSignature = RootSignature;
	PipelineStateDesc.VS = CreateVertexShader(&VertexShaderBlob);
	PipelineStateDesc.PS = CreatePixelShader(&PixelShaderBlob);
	PipelineStateDesc.RasterizerState = CreateRasterizerState();
	PipelineStateDesc.BlendState = CreateBlendState();
	PipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	PipelineStateDesc.InputLayout = CreateInputLayout();
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	PipelineStateDesc.SampleDesc.Count = 1;
	PipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	Device->CreateGraphicsPipelineState(&PipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&PipelineState);

	if (VertexShaderBlob)
		VertexShaderBlob->Release();

	if (PixelShaderBlob)
		PixelShaderBlob->Release();

	if (PipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] PipelineStateDesc.InputLayout.pInputElementDescs;
}

void Shader::OnPrepareRender(ID3D12GraphicsCommandList* CmdList) {
	if (PipelineState)
		CmdList->SetPipelineState(PipelineState);
}

void Shader::Render(ID3D12GraphicsCommandList* CmdList) {
	OnPrepareRender(CmdList);
}