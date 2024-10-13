#pragma once
#include <d3d12.h>
#include "CBVData.h"

typedef struct {
	ID3D12DescriptorHeap* Heap;
	ID3D12Resource* Buffer;
}HeapAndBuffer;

// flip CBV
extern HeapAndBuffer FlipHB;

void CreateCBVResource(ID3D12Device* Device);