#include "pch.h"
#include "Device.h"

void Device::Init()
{
	// DXGI => Direct X Graphics Infrastructure의 약자
	// 전체화면 모드 전환
	// 디스플레이 모드 전환 등
	::CreateDXGIFactory(IID_PPV_ARGS(&dxgi));

	// CreateDevice
	// Display Adapter(GPU)에 접근하기 위한 함수
	// adapter가 nullptr일 땐 기본 어댑터에 접근
	// Minimum Feature Level : 응용 프로그램이 요구하는 최소 사양
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
}
