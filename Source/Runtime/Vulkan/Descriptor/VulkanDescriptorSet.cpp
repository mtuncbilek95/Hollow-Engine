#include "VulkanDescriptorSet.h"

namespace Hollow
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice) : DescriptorSet(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
	}
}