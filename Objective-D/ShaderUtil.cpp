#include "ShaderUtil.h"

// 쉐이더

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

void Shader::OnPrepareRender(ID3D12GraphicsCommandList* CmdList, ID3D12PipelineState* PS) {
	if (PS)
		CmdList->SetPipelineState(PS);
}

// DepthTest가 true일 때만 깊이 검사 실행
void Shader::Render(ID3D12GraphicsCommandList* CmdList, bool DepthTest) {
	if (DepthTest)
		OnPrepareRender(CmdList, PipelineState);
	else
		OnPrepareRender(CmdList, PSDepthNone);
}


////////////////////////////////
// 이 프로젝트에서 현재 사용하는 쉐이더, 추후 변경될 수 있음
D3D12_INPUT_LAYOUT_DESC BasicObjectShader::CreateInputLayout() {
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

D3D12_SHADER_BYTECODE BasicObjectShader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//Shader//BasicShader.hlsl", "VSTexColor", "vs_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE BasicObjectShader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//Shader//BasicShader.hlsl", "PSTexColor", "ps_5_1", ShaderBlob));
}