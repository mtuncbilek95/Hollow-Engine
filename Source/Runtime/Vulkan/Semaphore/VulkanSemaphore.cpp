#include "VulkanSemaphore.h"

namespace Hollow
{
	VulkanSemaphore::VulkanSemaphore(const SharedPtr<VulkanDevice>& pDevice) : Semaphore(pDevice), mVkSemaphore(VK_NULL_HANDLE), 
		mVkDevice(pDevice->GetVkDevice())
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.flags = VkSemaphoreCreateFlags();

		CORE_ASSERT(vkCreateSemaphore(mVkDevice, &semaphoreInfo, nullptr, &mVkSemaphore) == VK_SUCCESS, "VulkanSemaphore", "Failed to create semaphore");
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		if (mVkSemaphore != VK_NULL_HANDLE)
			vkDestroySemaphore(mVkDevice, mVkSemaphore, nullptr);

		mVkSemaphore = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
