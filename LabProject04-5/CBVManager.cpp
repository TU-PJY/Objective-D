#include "CBVManager.h"
#include "DirectX_3D.h"

FlipInfo Flipinfo;
// texture flip
std::vector<HeapAndBuffer> FlipHB;
// image flip
HeapAndBuffer ImageFlipHB;


AlphaInfo Alphainfo;
// texture alpha
std::vector<HeapAndBuffer> AlphaHB;

// ������۷� ����� ���� �� ���� �����Ѵ�.
// �ʿ信 ���� ������ ����� �� ��ü�� ����� ���� �ִ�.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	FlipHB.reserve(2);
	for(int i = 0; i < 2; ++i)
		CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB[i]);

	// image flip
	// �̹��� ����� ��� ���������� �����¿� �����ؾ� �ϹǷ� �ϳ��� CBV�� ����ϴ�.
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), ImageFlipHB);

	// texture alpha
	AlphaHB.reserve(2);
	for(int i = 0; i < 2; ++i)
		CBVUtil::CreateCBV(Device, &Alphainfo, sizeof(Alphainfo), AlphaHB[i]);
}