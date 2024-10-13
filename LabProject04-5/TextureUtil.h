#pragma once
#include "DirectX_3D.h"

using Texture = ID3D12Resource*;

extern ID3D12Resource* CreateTextureResourceFromWICFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, ID3D12Resource** ppd3dUploadBuffer, D3D12_RESOURCE_STATES d3dResourceStates = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
extern void CreateTextureAndSRV(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszTextureFilename, ID3D12Resource** ppd3dTexture, ID3D12DescriptorHeap** ppd3dSrvDescriptorHeap, ID3D12DescriptorHeap** ppd3dSamplerDescriptorHeap);