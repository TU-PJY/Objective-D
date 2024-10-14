#pragma once
#include <d3d12.h>
#include <vector>
#include "CBVData.h"

typedef struct {
	std::vector<ID3D12DescriptorHeap*> Heap;
	std::vector<ID3D12Resource*> Buffer;
}HeapAndBuffer;

// flip CBV
extern HeapAndBuffer FlipHB;
extern HeapAndBuffer ImageFlipHB;

// ahpha CBV
extern HeapAndBuffer AlphaHB;

// light CBV
extern HeapAndBuffer LightHB;


void CreateCBVResource(ID3D12Device* Device);
inline void ReserveHB(HeapAndBuffer& HB_Struct, int ReserveSize) {
	HB_Struct.Buffer.reserve(ReserveSize);
	HB_Struct.Heap.reserve(ReserveSize);
}