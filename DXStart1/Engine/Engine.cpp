#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

void Engine::Init(const WindowInfo& wInfo)
{
    window = wInfo;
    ResizeWindow(wInfo.width, wInfo.height);

	viewport = { 0, 0, static_cast<FLOAT>(wInfo.width), static_cast<FLOAT>(wInfo.height), 0.0f, 1.0f };
	scissorRect = CD3DX12_RECT(0, 0, wInfo.width, wInfo.height);

    device = make_shared<Device>();
    cmdQueue = make_shared<CommandQueue>();
    swapChain = make_shared<SwapChain>();
    descHeap = make_shared<DescriptorHeap>();

    device->Init();
    cmdQueue->Init(device->GetDevice(), swapChain, descHeap);
    swapChain->Init(wInfo, device->GetDXGI(), cmdQueue->GetCommandQueue());
    descHeap->Init(device->GetDevice(), swapChain);
}

void Engine::Render()
{
    RenderBegin(); // Rendering 전처리
    
    // 오브젝트 렌더링
    
    RenderEnd(); // Redering 후처리
}

void Engine::RenderBegin()
{
    cmdQueue->RenderBegin(&viewport, &scissorRect);
}

void Engine::RenderEnd()
{
    cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
    window.width = width;
    window.height = height;

    RECT rect = { 0, 0, width, height };
    ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    ::SetWindowPos(window.hWnd, 0, 100, 100, width, height, 0);
}
