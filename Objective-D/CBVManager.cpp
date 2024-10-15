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
	// �ؽ�ó �ø� CBV ����
	FlipInfo texFlip[4] { {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	for (int i = 0; i < 4; ++i)
		CBVUtil::UpdateCBV(CmdList, &texFlip[i], sizeof(texFlip[i]), FlipHB, 3, i);

	// �̹��� �ø��� ����� CBV�� �����Ѵ�.
	CBVUtil::UpdateCBV(CmdList, &texFlip[2], sizeof(texFlip)[2], ImageFlipHB, 3, 0);

	// ���� ��� ���� CBV ���� �����Ѵ�.
	// 1�� �ε���: true, 0�� �ε���, false
	UseLightInfo Uselightinfo[2] { {0}, {1} };
	for(int i = 0; i < 2; ++i)
		CBVUtil::UpdateCBV(CmdList, &Uselightinfo[i], sizeof(Uselightinfo[i]), BoolLightHB, 6, i);

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
}