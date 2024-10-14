#pragma once
#include <d3d12.h>
#include <vector>
#include "CBVData.h"

typedef struct {
	ID3D12DescriptorHeap* Heap;
	ID3D12Resource* Buffer;
}HeapAndBuffer;

// flip CBV
extern std::vector<HeapAndBuffer> FlipHB;
extern HeapAndBuffer ImageFlipHB;

// ahpha CBV
extern std::vector<HeapAndBuffer> AlphaHB;

void CreateCBVResource(ID3D12Device* Device);