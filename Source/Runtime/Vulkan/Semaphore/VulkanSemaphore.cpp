#include "VulkanSemaphore.h"

namespace Hollow
{
	VulkanSemaphore::VulkanSemaphore(const SemaphoreDesc& desc, VulkanDevice* pDevice) : Semaphore(desc, pDevice)
	{
		  mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		  VkSemaphoreCreateInfo semaphoreInfo = {};
		  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		  semaphoreInfo.flags = desc.bSignalled ? VK_FENCE_CREATE_SIGNALED_BIT : VkSemaphoreCreateFlags();
		  semaphoreInfo.pNext = nullptr;

		  DEV_ASSERT(vkCreateSemaphore(mVkLogicalDevice, &semaphoreInfo, nullptr, &mVkSemaphore) == VK_SUCCESS, "VulkanSemaphore", "Failed to create semaphore.");
	}

	void VulkanSemaphore::OnShutdown() noexcept
	{
		if (mVkSemaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(mVkLogicalDevice, mVkSemaphore, nullptr);
			mVkSemaphore = VK_NULL_HANDLE;
		}
	}
}