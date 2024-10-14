#include "CBVManager.h"
#include "DirectX_3D.h"

FlipInfo Flipinfo;
HeapAndBuffer FlipHB; // texture flip
HeapAndBuffer ImageFlipHB; // image flip

AlphaInfo Alphainfo;
HeapAndBuffer AlphaHB; // texture alpha

LightInfo Lightinfo;
HeapAndBuffer LightHB;

// ������۷� ����� ���� �� ���� �����Ѵ�.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	ReserveHB(FlipHB, 2);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB, 2);

	// image flip
	ReserveHB(ImageFlipHB, 1);
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), ImageFlipHB, 1);

	// texture alpha
	ReserveHB(AlphaHB, 2);
	CBVUtil::CreateCBV(Device, &Alphainfo, sizeof(Alphainfo), AlphaHB, 2);

	// light data
	ReserveHB(LightHB, 1);
	CBVUtil::CreateCBV(Device, &Lightinfo, sizeof(Lightinfo), LightHB, 1);
}