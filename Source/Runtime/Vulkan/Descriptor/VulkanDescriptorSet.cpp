#include "VulkanDescriptorSet.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>

namespace Hollow
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice) : DescriptorSet(desc, pDevice), mVkDescriptorSet(VK_NULL_HANDLE), 
		mVkDevice(pDevice->GetVkDevice())
	{
		VkDescriptorSetLayout layout = static_cast<VulkanDescriptorLayout*>(desc.pLayout)->GetVkLayout();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = static_cast<VulkanDescriptorPool*>(desc.pOwnerPool)->GetVkDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		allocInfo.pNext = nullptr;

		CORE_ASSERT(vkAllocateDescriptorSets(mVkDevice, &allocInfo, &mVkDescriptorSet) == VK_SUCCESS, "VulkanLayout", "Failed to allocate descriptor set");

		mVkDescriptorPool = static_cast<VulkanDescriptorPool*>(desc.pOwnerPool)->GetVkDescriptorPool();
		mVkDescriptorLayout = layout;
	}

	void VulkanDescriptorSet::OnShutdown() noexcept
	{
		if(mVkDescriptorSet != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(mVkDevice, mVkDescriptorPool, 1, &mVkDescriptorSet);
			mVkDescriptorSet = VK_NULL_HANDLE;
		}

		mVkDescriptorPool = VK_NULL_HANDLE;
		mVkDescriptorLayout = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}