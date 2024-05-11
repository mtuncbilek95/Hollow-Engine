#include "VulkanCommandPool.h"

namespace Hollow
{
	VulkanCommandPool::VulkanCommandPool(const CommandPoolDesc& desc, const SharedPtr<VulkanDevice> pDevice) : CommandPool(desc, pDevice), 
		mVkDevice(pDevice->GetVkDevice()), mVkCommandPool(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = pDevice->GetQueueFamilyIndex(static_cast<GraphicsQueueType>(desc.PoolType));
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		CORE_ASSERT(vkCreateCommandPool(mVkDevice, &poolInfo, nullptr, &mVkCommandPool) == VK_SUCCESS, "VulkanCommandPool", "Failed to create command pool");
	}

	void VulkanCommandPool::OnShutdown() noexcept
	{
		if (mVkCommandPool != VK_NULL_HANDLE)
			vkDestroyCommandPool(mVkDevice, mVkCommandPool, nullptr);

		mVkCommandPool = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;

		CORE_LOG(HE_INFO, "VulkanCommandPool", "Command pool has been destroyed");
	}
}
