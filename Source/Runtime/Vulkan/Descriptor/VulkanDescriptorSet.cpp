#include "VulkanDescriptorSet.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>

namespace Hollow
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice) : DescriptorSet(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		VulkanDescriptorPool* pPool = static_cast<VulkanDescriptorPool*>(desc.pPool.get());
		VulkanDescriptorLayout* pLayout = static_cast<VulkanDescriptorLayout*>(desc.pLayout.get());

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pPool->GetVkDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		VkDescriptorSetLayout layout = pLayout->GetVkDescriptorLayout();
		allocInfo.pSetLayouts = &layout;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pNext = nullptr;

		DEV_ASSERT(vkAllocateDescriptorSets(mVkLogicalDevice, &allocInfo, &mVkDescriptorSet) == VK_SUCCESS, "VulkanDescriptorSet", 
			"Failed to allocate descriptor set");

		mVkDescriptorPool = pPool->GetVkDescriptorPool();
	}

	void VulkanDescriptorSet::OnShutdown() noexcept
	{
		if(mVkDescriptorSet != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(mVkLogicalDevice, mVkDescriptorPool, 1, &mVkDescriptorSet);
			mVkDescriptorSet = VK_NULL_HANDLE;
		}
	}
}