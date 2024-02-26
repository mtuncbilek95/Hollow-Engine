#include "VulkanCommandPool.h"

namespace Hollow
{
	VulkanCommandPool::VulkanCommandPool(const CommandPoolDesc& desc, VulkanDevice* pDevice) : CommandPool(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = pDevice->CatchQueueFamilyIndex(static_cast<GraphicsQueueType>(desc.Type));
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		DEV_ASSERT(vkCreateCommandPool(mVkLogicalDevice, &poolInfo, nullptr, &mVkCommandPool) == VK_SUCCESS, "VulkanCommandPool", "Failed to create command pool!");
	}

	void VulkanCommandPool::OnShutdown() noexcept
	{
		if(mVkCommandPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(mVkLogicalDevice, mVkCommandPool, nullptr);
			mVkCommandPool = VK_NULL_HANDLE;
		}
	}
}