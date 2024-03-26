#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

CommandQueue::~CommandQueue()
{
    ::CloseHandle(fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap)
{
    this->swapChain = swapChain;
    this->descHeap = descHeap;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue)); // Ŀ�ǵ� ť ����
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));

    // Ŀ�ǵ� ����Ʈ�� CLOSE / OPEN ���¸� �ݺ�
    // OPEN ���¿� ��ɾ �Ҵ��ϰ� CLOSE ��Ų �� GPU�� ��ɾ� ����
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));
    cmdList->Close();

    // Fence : CPU�� GPU ����ȭ ��ġ
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
    // �潺 �÷��� ����
    fenceValue++;

    // Ŀ�ǵ� ť�� �潺 ��ġ
    // GPU�� ��� ó���� �Ϸ��� ������ ��ٸ���
    cmdQueue->Signal(fence.Get(), fenceValue);

    // �潺�� ���� �ɷ��ִٸ�
    if (fence->GetCompletedValue() < fenceValue)
    {
        // �潺�� Ǯ�� �� ����Ǵ� �潺 �̺�Ʈ ����
        fence->SetEventOnCompletion(fenceValue, fenceEvent);
        // �潺 �̺�Ʈ�� ����� ������ ��ٸ���. �� CPU�� GPU�� ��ٸ��� ����
        ::WaitForSingleObject(fenceEvent, INFINITE);
    }
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect)
{
    cmdAlloc->Reset();
    cmdList->Reset(cmdAlloc.Get(), nullptr);

    // SwapChain�� �۵��ϴ� �踮��
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChain->GetCurrentBackBufferResource().Get(),
        D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���
        D3D12_RESOURCE_STATE_RENDER_TARGET // ���� �����
    ); 

    cmdList->ResourceBarrier(1, &barrier);

    // viewport�� scissor rect ����
    // ��� ����� �缳�� �� ������ ȣ�� �Ǿ�� �Ѵ�.
    cmdList->RSSetViewports(1, viewport);
    cmdList->RSSetScissorRects(1, scissorRect);

    // �������� ���� ����
    D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = descHeap->GetBackBufferView();
    cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
    cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChain->GetCurrentBackBufferResource().Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, // GPU �����
        D3D12_RESOURCE_STATE_PRESENT // ȭ�� ���
    );

    cmdList->ResourceBarrier(1, &barrier);
    cmdList->Close();

    // Ŀ�ǵ� ����Ʈ ����
    ID3D12CommandList* cmdListArr[] = { cmdList.Get() };
    cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

    // ���� ȭ�� ���
    swapChain->Present();

    WaitSync();

    swapChain->SwapIndex();
}
