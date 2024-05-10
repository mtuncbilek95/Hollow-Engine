#include "VulkanSemaphore.h"

namespace Hollow
{
	VulkanSemaphore::VulkanSemaphore(VulkanDevice* pDevice) : Semaphore(pDevice), mVkDevice(pDevice->GetVkDevice()), 
		mVkSemaphore(VK_NULL_HANDLE)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = VkSemaphoreCreateFlags();

		CORE_ASSERT(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkSemaphore) == VK_SUCCESS, "VulkanSemaphore", "Failed to create semaphore");
	}

	void VulkanSemaphore::OnShutdown() noexcept
	{
		if (mVkSemaphore != VK_NULL_HANDLE)
			vkDestroySemaphore(mVkDevice, mVkSemaphore, nullptr);

		mVkSemaphore = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanSemaphore", "Semaphore shutdown successfully");
	}

}