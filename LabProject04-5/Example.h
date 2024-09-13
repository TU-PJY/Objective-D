#pragma once
#include "GameObject.h"

class Object : public GameObject {
public:
	ObjectVector ov;
	XMFLOAT3 Position{}, Rotation{};

	Object() {
		Math::InitVector(ov.Up, ov.Right, ov.Look);
		SetColor(0.5, 0.5, 0.5);
		Position.z = 10.0;
	}

	XMFLOAT3 GetUp() {
		return ov.Up;
	}

	XMFLOAT3 GetRight() {
		return ov.Right;
	}

	XMFLOAT3 GetLook() {
		return ov.Look;
	}

	XMFLOAT3 GetPosition() {
		return Position;
	}



	void Update(float FT) {
		Math::UpdateVector(ov.Up, ov.Right, ov.Look, Rotation.x, Rotation.y, Rotation.z);
	}

	void Render(CommandList CmdList) {
		InitMatrix();
		Transform::Scale(ScaleMatrix, 2.0, 2.0, 2.0);
		Transform::Move(TranslateMatrix, 0.0, 0.0, 20.0);
		RenderMesh(CmdList, LightingShader, MeshPlayer);
	}
};