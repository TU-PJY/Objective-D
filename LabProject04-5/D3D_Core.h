#pragma once
#include "D3D_Header.h"
#include "Timer.h"

class D3D_Core {
public:
	D3D_Core();
	~D3D_Core();

	bool Create(HINSTANCE hInstance, HWND hMainWnd);
	void Destroy();

	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateCommandQueueAndList();

	void CreateRtvAndDsvDescriptorHeaps();

	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void CreateRenderTargetViewsAndDepthStencilView();

	void ChangeSwapChainState();

    void Init();
    void ReleaseObjects();

    void ProcessInput();
    void Routine();
    void Update();
	void Render(ID3D12GraphicsCommandList* CmdList);

	virtual void CreateShaderVariables();
	virtual void UpdateShaderVariables();
	virtual void ReleaseShaderVariables();

	void WaitForGpuComplete();
	void MoveToNextFrame();

	void MouseFunc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void KeyboardFunc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowsMessegeFunc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE					hInstance;
	HWND						hWnd; 

	int							CLIENT_WIDTH;
	int							CLIENT_HEIGHT;
        
	IDXGIFactory4				*DxgiFactory;
	IDXGISwapChain3				*DxgiSwapChain;
	ID3D12Device				*Device;

	bool						MSAA_4x_Option = false;
	UINT						MSAA_4x_QualityLevels = 8;

	static const UINT			SwapChainBuffers = 2;
	UINT						SwapChainBufferIndex;

	ID3D12Resource				*SwapChainBackBuffers[SwapChainBuffers];
	ID3D12DescriptorHeap		*RtvDescriptorHeap;
	UINT						RtvDescriptorIncrementSize;

	ID3D12Resource				*DepthStencilBuffer;
	ID3D12DescriptorHeap		*DsvDescriptorHeap;
	UINT						DsvDescriptorIncrementSize;

	ID3D12CommandAllocator		*CmdAllocator;
	ID3D12CommandQueue			*CmdQueue;
	ID3D12GraphicsCommandList	*CmdList;

	ID3D12Fence					*m_pd3dFence;
	UINT64						FenceValues[SwapChainBuffers];
	HANDLE						FenceEvent;

#if defined(_DEBUG)
	ID3D12Debug					*DebugController;
#endif

	POINT						PrevCursorPosition;

	Timer						Timer;
	_TCHAR						FrameRate[50];

public:
	TCHAR						WindowName[50]{};
};

