#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
    this->swapChain = swapChain;

    // Descriptor(DX12) = View(DX11)
    // RTV 생성
    // DX11의 
    // RTV => Render Target View, 
    // DSV => Depth Stencil View, 
    // CBV => Constant Buffer View,
    // SRV => Shader Resource View,
    // UAV => Unordered Access View
    rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
    rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
    rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvDesc.NodeMask = 0;

    // 같은 종류의 데이터를 배열로 관리
    // RTV : [] [] [] ...
    // DSV : [] [] [] ...
    // ...
    device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap));

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
    {
        rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
        device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, rtvHandle[i]);
        // Swap Chain에 있는 0번과 1번의 Render Target을 받아와 Render Target View 생성
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
    return GetRTV(swapChain->GetCurrentBackBufferIndex());
}
