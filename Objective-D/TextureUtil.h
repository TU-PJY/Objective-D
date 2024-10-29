#pragma once
#include "DirectX_3D.h"

class Texture {
private:
	ID3D12Resource* Tex{};
	ID3D12DescriptorHeap* SRV{};
	ID3D12DescriptorHeap* Sampler{};

public:
	Texture(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, wchar_t* FileName, int Type);
	void Render(ID3D12GraphicsCommandList* CmdList);
	void CreateTextureAndSRV(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszTextureFilename, ID3D12Resource** ppd3dTexture, ID3D12DescriptorHeap** ppd3dSrvDescriptorHeap, ID3D12DescriptorHeap** ppd3dSamplerDescriptorHeap, int Type);
	ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	ID3D12Resource* CreateTextureResourceFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
};