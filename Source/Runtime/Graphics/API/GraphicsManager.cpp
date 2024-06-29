#include "GraphicsManager.h"

namespace Hollow
{
	void GraphicsManager::SetMemoryBudget(const u64 hostMemoryBudget, const u64 deviceMemoryBudget) noexcept
	{
		GraphicsMemoryDesc hostMemoryDesc = {};
		hostMemoryDesc.MemoryType = GraphicsMemoryType::HostVisible;
		hostMemoryDesc.SizeInBytes = MB_TO_BYTE(hostMemoryBudget);
		mHostMemory = mGraphicsDevice->CreateGraphicsMemory(hostMemoryDesc);

		GraphicsMemoryDesc deviceMemoryDesc = {};
		deviceMemoryDesc.MemoryType = GraphicsMemoryType::DeviceLocal;
		deviceMemoryDesc.SizeInBytes = MB_TO_BYTE(deviceMemoryBudget);
		mDeviceMemory = mGraphicsDevice->CreateGraphicsMemory(deviceMemoryDesc);
	}

	void GraphicsManager::SetGraphicsDevice(SharedPtr<GraphicsDevice> device)
	{
		if (!mGraphicsDevice)
			mGraphicsDevice = device;

		GraphicsQueueDesc presentQueueDesc = {};
		presentQueueDesc.QueueType = GraphicsQueueType::Graphics;
		mPresentQueue = mGraphicsDevice->CreateQueue(presentQueueDesc);

		GraphicsQueueDesc computeQueueDesc = {};
		computeQueueDesc.QueueType = GraphicsQueueType::Compute;
		mComputeQueue = mGraphicsDevice->CreateQueue(computeQueueDesc);

		GraphicsQueueDesc transferQueueDesc = {};
		transferQueueDesc.QueueType = GraphicsQueueType::Transfer;
		mTransferQueue = mGraphicsDevice->CreateQueue(transferQueueDesc);
	}
}
