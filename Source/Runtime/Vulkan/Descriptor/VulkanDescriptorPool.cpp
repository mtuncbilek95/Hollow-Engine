#include "VulkanDescriptorPool.h"

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice) : DescriptorPool(desc, pDevice), mVkDescriptorPool(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
	}

	void VulkanDescriptorPool::OnShutdown() noexcept
	{
	}
}