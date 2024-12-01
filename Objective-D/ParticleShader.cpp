#include "ParticleShader.h"

// 오브젝트 출력용 쉐이더
D3D12_INPUT_LAYOUT_DESC Particle_Shader::CreateInputLayout() {
	UINT NumInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* InputElementDescs = new D3D12_INPUT_ELEMENT_DESC[NumInputElementDescs];

	InputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	InputElementDescs[2] = { "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC InputLayoutDesc;
	InputLayoutDesc.pInputElementDescs = InputElementDescs;
	InputLayoutDesc.NumElements = NumInputElementDescs;

	return(InputLayoutDesc);
}

D3D12_SHADER_BYTECODE Particle_Shader::CreateVertexShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//ParticleShader.hlsl", "VS_Main", "vs_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE Particle_Shader::CreatePixelShader(ID3DBlob** ShaderBlob) {
	return(Shader::CompileShaderFromFile(L"Resources//SystemResources//Shader//ParticleShader.hlsl", "PS_Main", "ps_5_1", ShaderBlob));
}

D3D12_SHADER_BYTECODE Particle_Shader::CreateGeometryShader(ID3DBlob** ShaderBlob) {
	return Shader::CompileShaderFromFile( L"Resources//SystemResources//Shader//ParticleShader.hlsl", "GS_Main", "gs_5_1", ShaderBlob);
}