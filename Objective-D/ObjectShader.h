#pragma once
#include "DirectX_3D.h"
#include "ShaderUtil.h"

class BasicObjectShader : public Shader {
public:
	BasicObjectShader() {}
	virtual ~BasicObjectShader() {}

	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ShaderBlob);
};
