#include "VulkanDescriptorPool.h"

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice) : DescriptorPool(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
	}
}