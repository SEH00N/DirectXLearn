#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"

class Engine
{
public:
	void Init(const WindowInfo& wInfo);
	
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	WindowInfo window;
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};

private:
	shared_ptr<Device> device;
	shared_ptr<CommandQueue> cmdQueue;
	shared_ptr<SwapChain> swapChain;
	shared_ptr<DescriptorHeap> descHeap;
	shared_ptr<RootSignature> rootSignature;

public:
	inline shared_ptr<Device> GetDevice() { return device; }
	inline shared_ptr<CommandQueue> GetCmdQueue() { return cmdQueue; }
	inline shared_ptr<SwapChain> GetSwapChain() { return swapChain; }
	inline shared_ptr<DescriptorHeap> GetDescHeap() { return descHeap; }
	inline shared_ptr<RootSignature> GetRootSignature() { return rootSignature; }
};

