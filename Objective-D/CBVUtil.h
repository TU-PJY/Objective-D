#pragma once
#include "DirectX_3D.h"
#include "CBVManager.h"

namespace CBVUtil {
	void CreateCBV(ID3D12Device* Device, void* Data, size_t DataSize, CBV& CBV_Struct, int CBV_Index);
	void UpdateCBV(ID3D12GraphicsCommandList* CmdList, void* Data, size_t DataSize, CBV& CBV_Struct, int CBVIndex);
	void InputCBV(ID3D12GraphicsCommandList* CmdList, CBV& CBV_Struct, int CBV_Index);
	void SetSignatureIndex(CBV& CBV_Struct, int RootSignatureIndex);
}