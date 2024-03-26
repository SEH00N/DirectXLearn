#include "pch.h"
#include "Device.h"

void Device::Init()
{
	// DXGI => Direct X Graphics Infrastructure�� ����
	// ��üȭ�� ��� ��ȯ
	// ���÷��� ��� ��ȯ ��
	::CreateDXGIFactory(IID_PPV_ARGS(&dxgi));

	// CreateDevice
	// Display Adapter(GPU)�� �����ϱ� ���� �Լ�
	// adapter�� nullptr�� �� �⺻ ����Ϳ� ����
	// Minimum Feature Level : ���� ���α׷��� �䱸�ϴ� �ּ� ���
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
}
