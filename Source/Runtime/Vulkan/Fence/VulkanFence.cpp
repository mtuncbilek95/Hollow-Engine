#include "VulkanFence.h"

namespace Hollow
{
	VulkanFence::VulkanFence(const FenceDesc& desc, const SharedPtr<VulkanDevice> pDevice) : Fence(desc, pDevice), mVkDevice(pDevice->GetVkDevice()),
		mVkFence(VK_NULL_HANDLE)
	{
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		if (desc.bSignalled)
			fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		else
			fenceCreateInfo.flags = 0;

		CORE_ASSERT(vkCreateFence(mVkDevice, &fenceCreateInfo, nullptr, &mVkFence) == VK_SUCCESS, "VulkanFence", "Failed to create fence");
	}

	VulkanFence::~VulkanFence()
	{
		if (mVkFence != VK_NULL_HANDLE)
			vkDestroyFence(mVkDevice, mVkFence, nullptr);

		mVkFence = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
