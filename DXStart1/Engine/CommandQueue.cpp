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

    device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue)); // 커맨드 큐 생성
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));

    // 커맨드 리스트는 CLOSE / OPEN 상태를 반복
    // OPEN 상태에 명령어를 할당하고 CLOSE 시킨 후 GPU에 명령어 전송
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&cmdList));
    cmdList->Close();

    // Fence : CPU와 GPU 동기화 장치
    device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
    fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
    // 펜스 플래그 증가
    fenceValue++;

    // 커맨드 큐에 펜스 설치
    // GPU가 명령 처리를 완료할 때까지 기다리기
    cmdQueue->Signal(fence.Get(), fenceValue);

    // 펜스가 현재 걸려있다면
    if (fence->GetCompletedValue() < fenceValue)
    {
        // 펜스가 풀릴 때 발행되는 펜스 이벤트 구독
        fence->SetEventOnCompletion(fenceValue, fenceEvent);
        // 펜스 이벤트가 발행될 때까지 기다리기. 즉 CPU가 GPU를 기다리는 상태
        ::WaitForSingleObject(fenceEvent, INFINITE);
    }
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* viewport, const D3D12_RECT* scissorRect)
{
    cmdAlloc->Reset();
    cmdList->Reset(cmdAlloc.Get(), nullptr);

    // SwapChain과 작동하는 배리어
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChain->GetCurrentBackBufferResource().Get(),
        D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
        D3D12_RESOURCE_STATE_RENDER_TARGET // 외주 결과물
    ); 

    cmdList->ResourceBarrier(1, &barrier);

    // viewport와 scissor rect 설정
    // 명령 목록이 재설정 될 때마다 호출 되어야 한다.
    cmdList->RSSetViewports(1, viewport);
    cmdList->RSSetScissorRects(1, scissorRect);

    // 렌더링할 버퍼 지정
    D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = descHeap->GetBackBufferView();
    cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
    cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        swapChain->GetCurrentBackBufferResource().Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, // GPU 결과물
        D3D12_RESOURCE_STATE_PRESENT // 화면 출력
    );

    cmdList->ResourceBarrier(1, &barrier);
    cmdList->Close();

    // 커맨드 리스트 수행
    ID3D12CommandList* cmdListArr[] = { cmdList.Get() };
    cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

    // 현재 화면 출력
    swapChain->Present();

    WaitSync();

    swapChain->SwapIndex();
}
