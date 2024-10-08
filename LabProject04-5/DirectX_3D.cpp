#include "DirectX_3D.h"

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

ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates) {
	ID3D12Resource* pd3dTexture = NULL;
	std::unique_ptr<uint8_t[]> decodedData;
	D3D12_SUBRESOURCE_DATA d3dSubresource;

	HRESULT hResult = DirectX::LoadWICTextureFromFileEx(pd3dDevice, pszFileName, 0, D3D12_RESOURCE_FLAG_NONE, WIC_LOADER_DEFAULT, &pd3dTexture, decodedData, d3dSubresource);

	D3D12_HEAP_PROPERTIES d3dHeapPropertiesDesc;
	::ZeroMemory(&d3dHeapPropertiesDesc, sizeof(D3D12_HEAP_PROPERTIES));
	d3dHeapPropertiesDesc.Type = D3D12_HEAP_TYPE_UPLOAD;
	d3dHeapPropertiesDesc.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	d3dHeapPropertiesDesc.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	d3dHeapPropertiesDesc.CreationNodeMask = 1;
	d3dHeapPropertiesDesc.VisibleNodeMask = 1;

	UINT64 nBytes = GetRequiredIntermediateSize(pd3dTexture, 0, 1);

	D3D12_RESOURCE_DESC d3dResourceDesc;
	::ZeroMemory(&d3dResourceDesc, sizeof(D3D12_RESOURCE_DESC));
	d3dResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; //Upload Heap에는 텍스쳐를 생성할 수 없음
	d3dResourceDesc.Alignment = 0;
	d3dResourceDesc.Width = nBytes;
	d3dResourceDesc.Height = 1;
	d3dResourceDesc.DepthOrArraySize = 1;
	d3dResourceDesc.MipLevels = 1;
	d3dResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	d3dResourceDesc.SampleDesc.Count = 1;
	d3dResourceDesc.SampleDesc.Quality = 0;
	d3dResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	d3dResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	pd3dDevice->CreateCommittedResource(&d3dHeapPropertiesDesc, D3D12_HEAP_FLAG_NONE, &d3dResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)ppd3dUploadBuffer);

	::UpdateSubresources(pd3dCommandList, pd3dTexture, *ppd3dUploadBuffer, 0, 0, 1, &d3dSubresource);

	D3D12_RESOURCE_BARRIER d3dResourceBarrier;
	::ZeroMemory(&d3dResourceBarrier, sizeof(D3D12_RESOURCE_BARRIER));
	d3dResourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	d3dResourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	d3dResourceBarrier.Transition.pResource = pd3dTexture;
	d3dResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	d3dResourceBarrier.Transition.StateAfter = d3dResourceStates;
	d3dResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	pd3dCommandList->ResourceBarrier(1, &d3dResourceBarrier);

	return(pd3dTexture);
}
