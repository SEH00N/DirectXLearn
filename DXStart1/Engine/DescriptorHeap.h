#pragma once
class DescriptorHeap
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

public:
	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return rtvHandle[idx]; }
	D3D12_CPU_DESCRIPTOR_HANDLE     GetBackBufferView();

private:
	ComPtr<ID3D12DescriptorHeap>    rtvHeap;
	uint32							rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE     rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<class SwapChain>		swapChain;

};

