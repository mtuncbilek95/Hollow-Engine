#include "VulkanQueue.h"

namespace Hollow
{
	VulkanQueue::VulkanQueue(const GraphicsQueueDesc& desc, VkQueue reservedQueue, SharedPtr<VulkanDevice> pDevice) : GraphicsQueue(desc, pDevice),
		mVkQueue(reservedQueue)
	{
		mQueueFamilyIndex = pDevice->GetQueueFamilyIndex(desc.QueueType);
	}

	void VulkanQueue::OnShutdown() noexcept
	{
		mVkQueue = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanQueue", "Queue shutdown successfully");
	}
}