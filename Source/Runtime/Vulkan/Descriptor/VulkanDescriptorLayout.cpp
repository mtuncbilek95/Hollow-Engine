#include "VulkanDescriptorLayout.h"

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* pDevice) : DescriptorLayout(desc, pDevice), mVkLayout(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
	}

	void VulkanDescriptorLayout::OnShutdown() noexcept
	{
	}
}