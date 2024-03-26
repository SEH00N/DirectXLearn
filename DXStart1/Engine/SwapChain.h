#pragma once

/*
* [외주 과정]
* - GPU에게 명령을 내리면 연산을 해 버퍼에 그린다
* - 그 버퍼를 받아 화면에 출력한다
* 
* - 버퍼를 제작해 GPU에 건네주고 다시 돌려받아 화면에 출력한다
* 
* - 근데 화면에 출력중이던 버퍼를 다시 GPU에게 건네 덮어쓰게 한다면?
* - 화면이 잘리게 될 것.
* 
* - 그래서 GPU가 그리는 도화지 버퍼 한개
* - 화면에 출력하는 버퍼 한개
* 
* - 이렇게 두개의 버퍼를 사용하는 것이 더블 버퍼링
*/

class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

public:
	inline ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return renderTargets[index]; }
	uint32 GetCurrentBackBufferIndex() { return backBufferIndex; }
	ComPtr<ID3D12Resource> GetCurrentBackBufferResource() { return renderTargets[backBufferIndex]; }

private:
	ComPtr<IDXGISwapChain>	swapChain;
	ComPtr<ID3D12Resource>	renderTargets[SWAP_CHAIN_BUFFER_COUNT];
	uint32					backBufferIndex = 0;
};

