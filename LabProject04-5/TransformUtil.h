#pragma once
#include "DirectX_3D.h"

namespace Transform {
	void Move(XMFLOAT4X4& Matrix, float X, float Y, float Z);
	void Rotate(XMFLOAT4X4& Matrix, float Pitch, float Yaw, float Roll);
	void Scale(XMFLOAT4X4& Matrix, float X, float Y, float Z);
}