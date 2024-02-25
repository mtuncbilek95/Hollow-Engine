#include "VulkanFence.h"

namespace Hollow
{
	VulkanFence::VulkanFence(const FenceDesc& desc, VulkanDevice* pDevice) : Fence(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = desc.bSignalled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		fenceInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateFence(mVkLogicalDevice, &fenceInfo, nullptr, &mVkFence) == VK_SUCCESS, "VulkanFence", "Failed to create fence.");
	}

	void VulkanFence::OnShutdown() noexcept
	{
		if (mVkFence != VK_NULL_HANDLE)
		{
			vkDestroyFence(mVkLogicalDevice, mVkFence, nullptr);
			mVkFence = VK_NULL_HANDLE;
		}
	}
}