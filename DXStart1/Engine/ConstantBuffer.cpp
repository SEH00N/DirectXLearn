#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
    if (cbvBuffer)
    {
        if (cbvBuffer != nullptr)
            cbvBuffer->Unmap(0, nullptr);

        cbvBuffer = nullptr;
    }
}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
    // 상수 버퍼는 256 바이트 배수로 만들어야 한다
    // 0 256 512 768
    elementSize = (size + 255) & ~255;
    elementCount = count;

    CreateBuffer();
}

void ConstantBuffer::Clear()
{
    currentIndex = 0;
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
    // 조건이 만족하지 않으면 크래시를 내는 디버깅 코드
    assert(currentIndex < elementSize);


    ::memcpy(&mappedBuffer[currentIndex * elementSize], buffer, size);

    D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(currentIndex);
    CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
    currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	return D3D12_GPU_VIRTUAL_ADDRESS();
}

void ConstantBuffer::CreateBuffer()
{
    uint32 bufferSize = elementSize * elementCount;
    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

    DEVICE->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&cbvBuffer));

    cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mappedBuffer));
}
