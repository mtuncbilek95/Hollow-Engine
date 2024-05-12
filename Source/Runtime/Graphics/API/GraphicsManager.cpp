#include "GraphicsManager.h"

namespace Hollow
{
	void GraphicsManager::SetMemoryBudget(const uint64 hostMemoryBudget, const uint64 deviceMemoryBudget) noexcept
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

	void GraphicsManager::SetResourceBudget(const uint64 resourceCount, const uint64 setCount) noexcept
	{
		DescriptorPoolDesc poolDesc = {};
		poolDesc.PoolSizes =
		{
			{DescriptorType::Sampler, resourceCount },
			{DescriptorType::UniformBuffer, resourceCount },
			{DescriptorType::SampledImage, resourceCount },
			{DescriptorType::StorageImage, resourceCount },
			{DescriptorType::StorageBuffer, resourceCount },
			{DescriptorType::UniformTexelBuffer, resourceCount },
			{DescriptorType::StorageTexelBuffer, resourceCount },
			{DescriptorType::InputAttachment, resourceCount }
		};

		poolDesc.MaxSets = setCount;
		mDescriptorPool = mGraphicsDevice->CreateDescriptorPool(poolDesc);
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
