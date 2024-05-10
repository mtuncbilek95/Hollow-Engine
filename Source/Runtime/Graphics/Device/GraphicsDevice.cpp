#include "GraphicsDevice.h"

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : Object(), mInstance(desc.Instance), mGraphicsQueueCount(desc.GraphicsQueueCount), 
		mComputeQueueCount(desc.ComputeQueueCount), mTransferQueueCount(desc.TransferQueueCount), mOwnedSwapchain(nullptr), mDeviceObjects(), mAdapter()
	{
		mInstance->EnumerateAdapters();

		SharedPtr<GraphicsAdapter> tempAdapter;
		for (auto& adapter : mInstance->GetAdapters())
		{
			tempAdapter = adapter->AdapterScore > tempAdapter->AdapterScore ? adapter : tempAdapter;
		}

		mAdapter = tempAdapter;

		CORE_LOG(HE_INFO, "Graphics Device", "%s has been selected as the primary adapter", mAdapter->ProductName.c_str());
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		return SharedPtr<Swapchain>();
	}
	SharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		return SharedPtr<Texture>();
	}
	SharedPtr<TextureBuffer> GraphicsDevice::CreateTextureBuffer(const TextureBufferDesc& desc)
	{
		return SharedPtr<TextureBuffer>();
	}
	SharedPtr<GraphicsMemory> GraphicsDevice::CreateGraphicsMemory(const GraphicsMemoryDesc& desc)
	{
		return SharedPtr<GraphicsMemory>();
	}
}
