#pragma once
#include "DirectX_3D.h"
#include "ConstantBuffers.h"

namespace CBVUtil {
	void Create(ID3D12Device* Device, void* Data, size_t DataSize, CBV& CBV_Struct, int CBV_Index);
	void Update(ID3D12GraphicsCommandList* CmdList, void* Data, size_t DataSize, CBV& CBV_Struct, int CBVIndex);
	void Input(ID3D12GraphicsCommandList* CmdList, CBV& CBV_Struct, int CBV_Index);
	void SetSignatureIndex(CBV& CBV_Struct, int RootSignatureIndex);
}