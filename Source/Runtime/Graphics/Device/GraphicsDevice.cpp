#include "GraphicsDevice.h"

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : mGraphicsAPI(desc.API), mGraphicsInstance(desc.Instance), mSwapchain(nullptr)
	{
		// Check for the best adapter
		GraphicsAdapter tempAdapter = {};
		for (auto& adapter : desc.Instance->GetAdapters())
		{
			tempAdapter = adapter.AdapterScore > tempAdapter.AdapterScore ? adapter : tempAdapter;
		}

		mGraphicsAdapter = tempAdapter;
		CORE_LOG(HE_VERBOSE, "GraphicsDevice", "Chosen Adapter: %s", mGraphicsAdapter.ProductName.c_str());
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		if (mSwapchain == nullptr)
			mSwapchain = CreateSwapchainImpl(desc);
		return mSwapchain;
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::CreateGraphicsQueue(const GraphicsQueueDesc& desc)
	{
		auto queue = CreateGraphicsQueueImpl(desc);
		mDeviceObjects.push_back(queue);
		return queue;
	}
}
