#include "CBVManager.h"
#include "DirectX_3D.h"

FlipInfo Flipinfo;
HeapAndBuffer FlipHB; // texture flip
HeapAndBuffer ImageFlipHB; // image flip

LightInfo Lightinfo;
HeapAndBuffer LightHB;  // light

UseLightInfo Uselightinfo;
HeapAndBuffer BoolLightHB;  // light bool info

// ������۷� ����� ���� �� ���� �����Ѵ�.
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
	// �ؽ�ó �ø� CBV ����
	FlipInfo texFlip{ 0, 0 }; // None
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 0);

	texFlip.X_Flip = 1; texFlip.Y_Flip = 0; // Horizontal
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 1);

	texFlip.X_Flip = 0; texFlip.Y_Flip = 1; // Vertical
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 2);

	texFlip.X_Flip = 1; texFlip.Y_Flip = 1; // Vertical and Horizontal
	CBVUtil::UpdateCBV(CmdList, &texFlip, sizeof(texFlip), FlipHB, 3, 3);

	// �̹��� �ø��� ����� CBV�� �����Ѵ�.
	FlipInfo flip{ 0, 1 };
	CBVUtil::UpdateCBV(CmdList, &flip, sizeof(flip), ImageFlipHB, 3, 0);

	// ���� �⺻�� CBV�� �����Ѵ�.
	LightInfo light{
		{1.0f, -1.0f, 0.0f },  // Light Position
		0.0,
		{0.7f, 0.7f, 0.7f },  // Light Color
		0.0,
		{0.4f, 0.4f, 0.4f },  // Ambient Light Color
		5.0,  // Shadow Strength
	};
	CBVUtil::UpdateCBV(CmdList, &light, sizeof(light), LightHB, 5, 0);

	// ���� ��� ���� CBV ���� �����Ѵ�.
	// 1�� �ε���: true, 0�� �ε���, false
	UseLightInfo Uselightinfo{ 1 };
	CBVUtil::UpdateCBV(CmdList, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 6, 1);
	Uselightinfo.UseLight = 0;
	CBVUtil::UpdateCBV(CmdList, &Uselightinfo, sizeof(Uselightinfo), BoolLightHB, 6, 0);
}