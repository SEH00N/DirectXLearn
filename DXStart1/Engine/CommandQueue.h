#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect);
	void RenderEnd();

public:
	inline ComPtr<ID3D12CommandQueue> GetCommandQueue() { return cmdQueue; }
	inline ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return cmdList; }

private:
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12CommandAllocator> cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	ComPtr<ID3D12Fence> fence;
	uint32 fenceValue = 0;
	HANDLE fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain> swapChain;
	shared_ptr<DescriptorHeap> descHeap;
};