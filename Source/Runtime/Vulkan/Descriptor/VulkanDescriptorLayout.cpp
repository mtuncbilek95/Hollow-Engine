#include "VulkanDescriptorLayout.h"

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* pDevice) : DescriptorLayout(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();
	}
}