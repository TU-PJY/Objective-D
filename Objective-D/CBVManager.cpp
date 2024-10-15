#include "CBVManager.h"
#include "DirectX_3D.h"

FlipInfo Flipinfo;
HeapAndBuffer FlipHB; // texture flip
HeapAndBuffer ImageFlipHB; // image flip

LightInfo Lightinfo;
HeapAndBuffer LightHB;  // light

UseLightInfo Uselightinfo;
HeapAndBuffer BoolLightHB;  // light bool info

// 상수버퍼로 사용할 버퍼 및 힙을 설정한다.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	// 0: None
	// 1: Horizontal
	// 2: Vertical
	// 3: Vertical and Horizontal
	ReserveHB(FlipHB, 4);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB, 4);

	ReserveHB(ImageFlipHB, 1);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), ImageFlipHB, 1);

	// light data
	ReserveHB(LightHB, 1);
	CBVUtil::CreateCBV(Device, &Lightinfo, sizeof(Lightinfo), LightHB, 1);

	// light bool info
	// 1 : true, 0: false
	ReserveHB(BoolLightHB, 2);
	CBVUtil::CreateCBV(Device, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 2);
}

void SetCBVValue(ID3D12GraphicsCommandList* CmdList) {
	// 텍스처 플립 CBV 설정
	FlipInfo texFlip{ 0, 0 }; // None
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 0);

	texFlip.X_Flip = 1; texFlip.Y_Flip = 0; // Horizontal
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 1);

	texFlip.X_Flip = 0; texFlip.Y_Flip = 1; // Vertical
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 2);

	texFlip.X_Flip = 1; texFlip.Y_Flip = 1; // Vertical and Horizontal
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 3);

	// 이미지 플립에 사용할 CBV를 설정한다.
	FlipInfo flip{ 0, 1 };
	CBVUtil::UpdateCBV(CmdList, &flip, sizeof(flip), ImageFlipHB, 3, 0);

	// 조명 기본값 CBV를 설정한다.
	LightInfo light{
		{1.0f, -1.0f, 0.0f },  // Light Position
		0.0,
		{0.7f, 0.7f, 0.7f },  // Light Color
		0.0,
		{0.4f, 0.4f, 0.4f },  // Ambient Light Color
		5.0,  // Shadow Strength
	};
	CBVUtil::UpdateCBV(CmdList, &light, sizeof(light), LightHB, 5, 0);

	// 조명 사용 여부 CBV 값을 설정한다.
	// 1번 인덱스: true, 0번 인덱스, false
	UseLightInfo Uselightinfo{ 1 };
	CBVUtil::UpdateCBV(CmdList, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 6, 1);
	Uselightinfo.UseLight = 0;
	CBVUtil::UpdateCBV(CmdList, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 6, 0);
}