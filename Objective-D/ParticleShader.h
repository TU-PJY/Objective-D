#pragma once
#include "ShaderUtil.h"

class Particle_Shader : public Shader {
public:
	Particle_Shader() {}
	virtual ~Particle_Shader() {}

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** ShaderBlob);
};
#pragma once
