#pragma once
#include "DirectX_3D.h"

namespace Transform {
	void Move(XMFLOAT4X4& Matrix, float X, float Y, float Z);
	void Rotate(XMFLOAT4X4& Matrix, float RotationValue, float X, float Y, float Z);
	void Rotate(XMFLOAT4X4& Matrix, float RotationX, float RotationY, float RotationZ);
	void RotateYX(XMFLOAT4X4& Matrix, float RotationX, float RotationY);
	void Scale(XMFLOAT4X4& Matrix, float X, float Y, float Z);
	void ImageAspect(XMFLOAT4X4& Matrix, int Width, int Height);
}