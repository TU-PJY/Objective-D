#include "CBVUtil.h"

// ���̴��� ���Ǵ� ��� ���۸� �����Ѵ�
void CBVUtil::Create(ID3D12Device* Device, void* Data, size_t DataSize, CBV& CBV_Struct, int CBV_Index) {
	UINT64 cbSize = (DataSize + 255) & ~255;

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

	Device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&CBV_Struct.Buffer[CBV_Index])
	);

	void* pCbData = nullptr;  // void*�� ����
	D3D12_RANGE readRange = { 0, 0 };  // �б� ������ �����Ƿ� 0���� ����
	CBV_Struct.Buffer[CBV_Index]->Map(0, &readRange, reinterpret_cast<void**>(&pCbData));
	memcpy(pCbData, Data, DataSize);
	CBV_Struct.Buffer[CBV_Index]->Unmap(0, nullptr);
}

// CBV�� ���̴��� �����Ѵ�.
void CBVUtil::Input(ID3D12GraphicsCommandList* CmdList, CBV& CBV_Struct, int CBV_Index) {
	CmdList->SetGraphicsRootConstantBufferView(CBV_Struct.SignatureIndex, CBV_Struct.Buffer[CBV_Index]->GetGPUVirtualAddress());
}