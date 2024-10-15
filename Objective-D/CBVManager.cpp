#include "CBVManager.h"
#include "DirectX_3D.h"

CBV FlipCBV; // texture flip
CBV ImageFlipCBV; // image flip
CBV LightCBV;  // light
CBV BoolLightCBV;  // light bool info

// 상수버퍼로 사용할 버퍼 및 힙을 설정한다.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	FlipInfo TexFlip[4]{ {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	ReserveCBV(FlipCBV, 4);
	for (int i = 0; i < 4; ++i)
		CBVUtil::CreateCBV(Device, &TexFlip[i], sizeof(TexFlip[i]), FlipCBV, i);

	// image flip
	ReserveCBV(ImageFlipCBV, 1);
	CBVUtil::CreateCBV(Device, &TexFlip[2], sizeof(TexFlip)[2], ImageFlipCBV, 0);

	// light data
	LightInfo Light{
		{1.0f, -1.0f, 0.0f },  // Light Position
		0.0,
		{0.7f, 0.7f, 0.7f },  // Light Color
		0.0,
		{0.4f, 0.4f, 0.4f },  // Ambient Light Color
		5.0,  // Shadow Strength
	};
	ReserveCBV(LightCBV, 1);
	CBVUtil::CreateCBV(Device, &Light, sizeof(Light), LightCBV, 0);

	// light bool info
	UseLightInfo Uselightinfo[2]{ {0}, {1} };
	ReserveCBV(BoolLightCBV, 2);
	for (int i = 0; i < 2; ++i)
		CBVUtil::CreateCBV(Device, &Uselightinfo[i], sizeof(Uselightinfo[i]), BoolLightCBV, i);
}