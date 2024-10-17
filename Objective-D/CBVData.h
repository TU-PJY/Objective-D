#pragma once
#include <DirectXMath.h>
// 여기에 CBV로 사용할 구조체를 정의한다.

// texture flip
struct FlipInfo {
	int X_Flip;
	int Y_Flip;
};

// texture alpha
struct AlphaInfo {
	float AlphaValue;
};

// light data
struct LightInfo {
	DirectX::XMFLOAT3 gLightDirection;  // 12 bytes
	float padding1;                     // 4 bytes (패딩을 추가하여 16바이트 정렬)

	DirectX::XMFLOAT3 gLightColor;      // 12 bytes
	float padding2;                     // 4 bytes (16바이트 정렬)

	DirectX::XMFLOAT3 gAmbientColor;    // 12 bytes
	float gShadowStrength;              // 4 bytes (이로 인해 16바이트 정렬 완료)
};

struct UseLightInfo {
	int UseLight;
};