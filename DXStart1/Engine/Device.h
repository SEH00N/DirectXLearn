#pragma once
class Device
{
public:
	void Init();

public:
	// Comptr => dx의 객체를 다루기 위한 스마트 포인터
	ComPtr<IDXGIFactory> GetDXGI() { return dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return device; }

private:
	ComPtr<ID3D12Debug> debugController;
	ComPtr<IDXGIFactory> dxgi;
	ComPtr<ID3D12Device> device;

};

