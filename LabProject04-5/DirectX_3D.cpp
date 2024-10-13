#include "DirectX_3D.h"
#include "CBVManager.h"

UINT CbvSrvDescriptorIncrementSize = 0;

ID3D12Resource* CreateBufferResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, void* Data, UINT Byte, D3D12_HEAP_TYPE HeapType, D3D12_RESOURCE_STATES ResourceState, ID3D12Resource** UploadBuffer) {
	ID3D12Resource* Buffer = NULL;

	D3D12_HEAP_PROPERTIES HeapPropertiesDesc;
	::ZeroMemory(&HeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	HeapPropertiesDesc.Type = HeapType;
	HeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapPropertiesDesc.CreationNodeMask = 1;
	HeapPropertiesDesc.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC ResourceDesc;
	::ZeroMemory(&ResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Alignment = 0;
	ResourceDesc.Width = Byte;
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.SampleDesc.Quality = 0;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_RESOURCE_STATES ResourceInitialStates = D3D12_RESOURCE_STATE_COMMON;
	//	if (HeapType == D3D12_HEAP_TYPE_UPLOAD) ResourceInitialStates = D3D12_RESOURCE_STATE_GENERIC_READ;
	//	else if (HeapType == D3D12_HEAP_TYPE_READBACK) ResourceInitialStates = D3D12_RESOURCE_STATE_COPY_DEST;

	HRESULT hResult = Device->CreateCommittedResource(&HeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &ResourceDesc, ResourceInitialStates, NULL, __uuidof(ID3D12Resource), (void**)&Buffer);

	if (Data) {
		switch (HeapType) {
		case D3D12_HEAP_TYPE_DEFAULT:
		{
			if (UploadBuffer) {
				HeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
				Device->CreateCommittedResource(&HeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)UploadBuffer);

				D3D12_RANGE ReadRange = { 0, 0 };
				UINT8* pBufferDataBegin = NULL;
				(*UploadBuffer)->Map(0, &ReadRange, (void**)&pBufferDataBegin);
				memcpy(pBufferDataBegin, Data, Byte);
				(*UploadBuffer)->Unmap(0, NULL);

				CmdList->CopyResource(Buffer, *UploadBuffer);

				D3D12_RESOURCE_BARRIER ResourceBarrier;
				::ZeroMemory(&ResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
				ResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				ResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				ResourceBarrier.Transition.pResource = Buffer;
				ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
				ResourceBarrier.Transition.StateAfter = ResourceState;
				ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				CmdList->ResourceBarrier(1, &ResourceBarrier);
			}
		}
		break;

		case D3D12_HEAP_TYPE_UPLOAD:
		{
			D3D12_RANGE ReadRange = { 0, 0 };
			UINT8* BufferDataBegin = NULL;
			Buffer->Map(0, &ReadRange, (void**)&BufferDataBegin);
			memcpy(BufferDataBegin, Data, Byte);
			Buffer->Unmap(0, NULL);
		}
		break;

		case D3D12_HEAP_TYPE_READBACK:
			break;
		}
	}

	return(Buffer);
}

// 쉐이더에 사용되는 루트 상수를 생성한다
void CBVUtil::CreateCBV(ID3D12Device* Device, void* Data, size_t DataSize, HeapAndBuffer& HAB_Struct) {
	// 데이터 크기를 올바르게 받아오기 위해 dataSize를 명시적으로 추가
	UINT64 cbSize = (DataSize + 255) & ~255;  // 256바이트 단위로 정렬

	D3D12_HEAP_PROPERTIES heapProps = {};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = cbSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	// 상수 버퍼 리소스 생성
	Device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&HAB_Struct.Buffer)
	);

	// 상수 버퍼 매핑
	void* pCbData = nullptr;  // void*로 매핑
	D3D12_RANGE readRange = { 0, 0 };  // 읽기 범위가 없으므로 0으로 설정
	HAB_Struct.Buffer->Map(0, &readRange, reinterpret_cast<void**>(&pCbData));

	// 데이터 복사 (void*에서 특정 타입으로 캐스팅 후 데이터 복사)
	memcpy(pCbData, Data, DataSize);

	// 매핑 해제
	HAB_Struct.Buffer->Unmap(0, nullptr);

	// 디스크립터 힙 생성
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&HAB_Struct.Heap));

	// CBV 생성
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = HAB_Struct.Buffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = cbSize;  // 256바이트 정렬된 크기

	// 디스크립터 힙에 CBV를 생성
	Device->CreateConstantBufferView(&cbvDesc, HAB_Struct.Heap->GetCPUDescriptorHandleForHeapStart());
}

// 루트 상수를 업데이트 한다
void CBVUtil::UpdateCBV(void* Data, size_t DataSize, HeapAndBuffer& HAB_Struct) {
	// 상수 버퍼를 다시 매핑하여 값을 업데이트
	void* pCbData = nullptr;  // 매핑을 위한 void* 타입
	D3D12_RANGE readRange = { 0, 0 };  // 읽기 범위가 없으므로 0으로 설정
	HAB_Struct.Buffer->Map(0, &readRange, reinterpret_cast<void**>(&pCbData));

	// 상수 버퍼에 새로운 데이터를 복사 (Data의 크기를 명시적으로 지정)
	memcpy(pCbData, Data, DataSize);

	// 매핑 해제
	HAB_Struct.Buffer->Unmap(0, nullptr);
}