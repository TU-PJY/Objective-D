#pragma once
#include <d3d12.h>
#include <vector>
#include "CBVData.h"

typedef struct {
	std::vector<ID3D12DescriptorHeap*> Heap;
	std::vector<ID3D12Resource*> Buffer;
	int SignatureIndex;
}CBV;

// flip CBV
extern CBV FlipCBV;

// light CBV
extern CBV LightCBV;

// light bool info CBV
extern CBV BoolLightCBV;


// CBV 생성 관련 함수들
void CreateCBVResource(ID3D12Device* Device);
inline void ReserveCBV(CBV& CBV_Struct, int ReserveSize) {
	CBV_Struct.Buffer.reserve(ReserveSize);
	CBV_Struct.Heap.reserve(ReserveSize);
}