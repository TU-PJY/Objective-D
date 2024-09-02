#pragma once
#include "ShaderUtil.h"
#include "MeshUtil.h"

class Shader;

class GameObject {
public:
	XMFLOAT4X4 TransformMatrix{};
	XMFLOAT4X4 RotateMatrix{};
	XMFLOAT4X4 ScaleMatrix{};

	XMFLOAT3 ModelColor{};

	XMFLOAT3 Look{ 0.0, 0.0, 1.0 };
	XMFLOAT3 Up{ 0.0, 1.0, 0.0 };
	XMFLOAT3 Right{ 1.0, 0.0, 0.0 };

	Shader* ObjectShader{};

	std::string ObjectTag{};
	bool DeleteMark{};


};