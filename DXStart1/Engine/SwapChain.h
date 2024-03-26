#pragma once

/*
* [���� ����]
* - GPU���� ����� ������ ������ �� ���ۿ� �׸���
* - �� ���۸� �޾� ȭ�鿡 ����Ѵ�
* 
* - ���۸� ������ GPU�� �ǳ��ְ� �ٽ� �����޾� ȭ�鿡 ����Ѵ�
* 
* - �ٵ� ȭ�鿡 ������̴� ���۸� �ٽ� GPU���� �ǳ� ����� �Ѵٸ�?
* - ȭ���� �߸��� �� ��.
* 
* - �׷��� GPU�� �׸��� ��ȭ�� ���� �Ѱ�
* - ȭ�鿡 ����ϴ� ���� �Ѱ�
* 
* - �̷��� �ΰ��� ���۸� ����ϴ� ���� ���� ���۸�
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

