#include "VulkanCommandPool.h"

namespace Hollow
{
	VulkanCommandPool::VulkanCommandPool(const CommandPoolDesc& desc, VulkanDevice* pDevice) : CommandPool(desc, pDevice),
		mVkCommandPool(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = pDevice->GetQueueFamilyIndex(static_cast<GraphicsQueueType>(desc.PoolType));
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		CORE_ASSERT(vkCreateCommandPool(pDevice->GetVkDevice(), &poolInfo, nullptr, &mVkCommandPool) == VK_SUCCESS, "VulkanCommandBuffer", "Failed to create command pool");
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