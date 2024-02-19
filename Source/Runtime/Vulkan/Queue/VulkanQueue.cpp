#include "VulkanQueue.h"

namespace Hollow
{
	VulkanQueue::VulkanQueue(const GraphicsQueueDesc& desc, VulkanDevice* pDevice) : GraphicsQueue(desc)
	{
		mVkQueue = pDevice->CatchGraphicsQueue(desc.Type);
		mQueueFamilyIndex = pDevice->CatchQueueFamilyIndex(desc.Type);
	}

	void VulkanQueue::OnShutdown() noexcept
	{
		// Free the queue
		mVkQueue = VK_NULL_HANDLE;
	}
}
