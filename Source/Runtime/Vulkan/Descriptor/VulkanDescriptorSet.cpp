#include "VulkanDescriptorSet.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>

namespace Hollow
{
	VulkanDescriptorSet::VulkanDescriptorSet(const DescriptorSetDesc& desc, const SharedPtr<VulkanDevice>& pDevice) : DescriptorSet(desc, pDevice), 
		mVkDescriptorSet(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		VkDescriptorSetLayout layout = std::static_pointer_cast<VulkanDescriptorLayout>(desc.pLayout)->GetVkDescriptorLayout();
		VkDescriptorPool pool = std::static_pointer_cast<VulkanDescriptorPool>(desc.pOwnerPool)->GetVkDescriptorPool();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		allocInfo.pNext = nullptr;

		CORE_ASSERT(vkAllocateDescriptorSets(mVkDevice, &allocInfo, &mVkDescriptorSet) == VK_SUCCESS, "VulkanLayout", "Failed to allocate descriptor set");

		mVkDescriptorPool = pool;
		mVkDescriptorLayout = layout;
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		if (mVkDescriptorSet != VK_NULL_HANDLE)
		{
			vkFreeDescriptorSets(mVkDevice, mVkDescriptorPool, 1, &mVkDescriptorSet);
			mVkDescriptorSet = VK_NULL_HANDLE;
		}

		mVkDescriptorPool = VK_NULL_HANDLE;
		mVkDescriptorLayout = VK_NULL_HANDLE;
		mVkDevice = VK_NULL_HANDLE;
	}
}
