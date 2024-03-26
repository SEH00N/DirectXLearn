#pragma once
class Device
{
public:
	void Init();

public:
	// Comptr => dx�� ��ü�� �ٷ�� ���� ����Ʈ ������
	ComPtr<IDXGIFactory> GetDXGI() { return dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return device; }

private:
	ComPtr<ID3D12Debug> debugController;
	ComPtr<IDXGIFactory> dxgi;
	ComPtr<ID3D12Device> device;

};

