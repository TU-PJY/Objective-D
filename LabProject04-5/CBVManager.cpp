#include "CBVManager.h"
#include "DirectX_3D.h"

// texture flip
HeapAndBuffer FlipHB;
FlipInfo Flipinfo;

// texture alpha
HeapAndBuffer AlphaHB;
AlphaInfo Alphainfo;

// ��Ʈ����� ����� ���� �� ���� �����Ѵ�.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB);

	// texture alpha
	CBVUtil::CreateCBV(Device, &Alphainfo, sizeof(Alphainfo), AlphaHB);
}