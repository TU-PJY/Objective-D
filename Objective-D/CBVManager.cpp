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
	ReserveHB(FlipHB, 4);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB, 4);

	// image flip
	ReserveHB(ImageFlipHB, 1);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), ImageFlipHB, 1);

	// light data
	ReserveHB(LightHB, 1);
	CBVUtil::CreateCBV(Device, &Lightinfo, sizeof(Lightinfo), LightHB, 1);

	// light bool info
	ReserveHB(BoolLightHB, 2);
	CBVUtil::CreateCBV(Device, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 2);
}

void SetCBVValue(ID3D12GraphicsCommandList* CmdList) {
	// 텍스처 플립 CBV 설정
	FlipInfo texFlip[4] { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	for (int i = 0; i < 4; ++i)
		CBVUtil::UpdateCBV(CmdList, &texFlip[i], sizeof(texFlip[i]), FlipHB, 3, i);

	// 이미지 플립에 사용할 CBV를 설정한다.
	CBVUtil::UpdateCBV(CmdList, &texFlip[2], sizeof(texFlip)[2], ImageFlipHB, 3, 0);

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
	UseLightInfo Uselightinfo[2]{ {0}, {1} };
	for (int i = 0; i < 2; ++i)
		CBVUtil::UpdateCBV(CmdList, &Uselightinfo[i], sizeof(Uselightinfo[i]), BoolLightHB, 6, i);
}