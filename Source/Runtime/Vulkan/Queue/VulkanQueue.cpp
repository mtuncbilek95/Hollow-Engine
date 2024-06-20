#include "VulkanQueue.h"

namespace Hollow
{
	VulkanQueue::VulkanQueue(const GraphicsQueueDesc& desc, VkQueue reservedQueue, SharedPtr<VulkanDevice> pDevice) : GraphicsQueue(desc, pDevice),
		mVkQueue(reservedQueue)
	{
		mQueueFamilyIndex = pDevice->GetQueueFamilyIndex(desc.QueueType);
	}

	VulkanQueue::~VulkanQueue()
	{
		mVkQueue = VK_NULL_HANDLE;
	}
}