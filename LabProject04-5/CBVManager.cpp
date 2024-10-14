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

// 루트상수로 사용할 버퍼 및 힙을 설정한다.
// 필요에 따라 여러개 만들어 각 객체에 사용할 수도 있다.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	FlipHB.reserve(2);
	for(int i = 0; i < 2; ++i)
		CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), FlipHB[i]);

	// image flip
	// 이미지 출력의 경우 공통적으로 상하좌우 반전해야 하므로 하나의 CBV로 충분하다.
	CBVUtil::CreateCBV(Device, &Flipinfo, sizeof(Flipinfo), ImageFlipHB);

	// texture alpha
	AlphaHB.reserve(2);
	for(int i = 0; i < 2; ++i)
		CBVUtil::CreateCBV(Device, &Alphainfo, sizeof(Alphainfo), AlphaHB[i]);
}