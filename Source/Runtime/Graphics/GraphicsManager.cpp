#include "GraphicsManager.h"

namespace Hollow
{
	GraphicsDevice* GraphicsManager::GetGraphicsDevice()
	{
		if (mGraphicsDevice == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no graphics device initialized.");

		return mGraphicsDevice;
	}

	sharedPtr<GraphicsQueue> GraphicsManager::GetPresentQueue() const noexcept
	{
		if (mPresentQueue == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no present queue initialized.");

		return mPresentQueue;
	}

	sharedPtr<GraphicsQueue> GraphicsManager::GetComputeQueue() const noexcept
	{
		if (mComputeQueue == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no compute queue initialized.");

		return mComputeQueue;
	}

	sharedPtr<GraphicsQueue> GraphicsManager::GetTransferQueue() const noexcept
	{
		if (mTransferQueue == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no transfer queue initialized.");

		return mTransferQueue;
	}

	sharedPtr<GraphicsMemory> GraphicsManager::GetHostMemory() const noexcept
	{
		if (mHostMemory == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no host memory initialized.");

		return mHostMemory;
	}

	sharedPtr<GraphicsMemory> GraphicsManager::GetDeviceMemory() const noexcept
	{
		if (mDeviceMemory == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no device memory initialized.");

		return mDeviceMemory;
	}

	void GraphicsManager::CreateInternalResources()
	{
		if (mGraphicsDevice == nullptr)
			CORE_LOG(HE_ERROR, "GraphicsManager", "There is no graphics device initialized.");

		GraphicsQueueDesc presentQueueDesc = {};
		presentQueueDesc.QueueType = GraphicsQueueType::Graphics;
		mPresentQueue = mGraphicsDevice->CreateGraphicsQueue(presentQueueDesc);

		GraphicsQueueDesc computeQueueDesc = {};
		computeQueueDesc.QueueType = GraphicsQueueType::Compute;
		mComputeQueue = mGraphicsDevice->CreateGraphicsQueue(computeQueueDesc);

		GraphicsQueueDesc transferQueueDesc = {};
		transferQueueDesc.QueueType = GraphicsQueueType::Transfer;
		mTransferQueue = mGraphicsDevice->CreateGraphicsQueue(transferQueueDesc);

		GraphicsMemoryDesc hostMemoryDesc = {};
		hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
		hostMemoryDesc.SizeInBytes = MB_TO_BYTE(100);
		mHostMemory = mGraphicsDevice->CreateGraphicsMemory(hostMemoryDesc);

		GraphicsMemoryDesc deviceMemoryDesc = {};
		deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
		deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(100);
		mDeviceMemory = mGraphicsDevice->CreateGraphicsMemory(deviceMemoryDesc);

		CORE_LOG(HE_VERBOSE, "GraphicsManager", "Internal resources have been created.");
	}

	void GraphicsManager::SetGraphicsDevice(GraphicsDevice* graphicsDevice)
	{
		if (mGraphicsDevice == nullptr)
			mGraphicsDevice = graphicsDevice;

		CORE_LOG(HE_VERBOSE, "GraphicsManager", "Graphics device has been initialized and set.");
	}
}
