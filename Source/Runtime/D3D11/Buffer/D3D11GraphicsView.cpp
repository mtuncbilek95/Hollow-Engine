#include "D3D11GraphicsView.h"

#include <Runtime/D3D11/Buffer/D3D11BufferUtils.h>

namespace Hollow
{
	D3D11GraphicsView::D3D11GraphicsView(const GraphicsViewDesc& desc, ID3D11Device* pDevice) : GraphicsView(desc)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11BufferUtils::GetResourceUsage(desc.UsageType);
		bufferDesc.BindFlags = D3D11BufferUtils::GetBufferUsage(desc.Usage);
		bufferDesc.CPUAccessFlags = D3D11BufferUtils::GetBufferCPUAccess(desc.CPUAccess);
		bufferDesc.MiscFlags = desc.MiscFlags;

		if (desc.SizeInBytes % 64 == 0) {
			bufferDesc.ByteWidth = desc.SizeInBytes;
		}
		else {
			bufferDesc.ByteWidth = desc.SizeInBytes + (64 - desc.SizeInBytes % 64);
		}

		bufferDesc.StructureByteStride = desc.StructureByteStride;

		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = desc.pInitialData;

		DEV_ASSERT(SUCCEEDED(pDevice->CreateBuffer(&bufferDesc, desc.pInitialData ? &subresourceData : nullptr, mD3DBuffer.GetAddressOf())),
			"D3D11GraphicsBuffer", "Failed to create D3D11GraphicsBuffer");
	}
}
