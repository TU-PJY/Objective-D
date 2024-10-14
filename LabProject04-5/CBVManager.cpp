#include "CBVManager.h"
#include "DirectX_3D.h"

FlipInfo Flipinfo;
HeapAndBuffer FlipHB; // texture flip
HeapAndBuffer ImageFlipHB; // image flip

AlphaInfo Alphainfo;
HeapAndBuffer AlphaHB; // texture alpha

// 상수버퍼로 사용할 버퍼 및 힙을 설정한다.
// 필요에 따라 여러개 만들어 각 객체에 사용할 수도 있다.
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
}