#include "VulkanDescriptorSet.h"

namespace Hollow
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice) : DescriptorSet(desc, pDevice), mVkDescriptorSet(VK_NULL_HANDLE), 
		mVkDevice(pDevice->GetVkDevice())
	{
	}

	void VulkanDescriptorSet::OnShutdown() noexcept
	{
	}
}