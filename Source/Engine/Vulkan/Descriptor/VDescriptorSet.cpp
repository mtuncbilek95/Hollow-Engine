#include "VDescriptorSet.h"

#include <Engine/Vulkan/Descriptor/VDescriptorLayout.h>
#include <Engine/Vulkan/Descriptor/VDescriptorPool.h>

namespace Hollow
{
	VDescriptorSet::VDescriptorSet(const DescriptorSetDesc& desc, const WeakPtr<VDevice>& pDevice) : DescriptorSet(desc, pDevice),
		mVkDescriptorSet(VK_NULL_HANDLE), mDevice(pDevice.lock()->GetVkDevice())
	{
		VkDescriptorSetLayout layout = desc.pLayout.lock()->GetSharedPtrAs<VDescriptorLayout>()->GetVkDescriptorLayout();
		VkDescriptorPool pool = desc.pOwnerPool.lock()->GetSharedPtrAs<VDescriptorPool>()->GetVkDescriptorPool();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;
		allocInfo.pNext = nullptr;

		CORE_ASSERT(vkAllocateDescriptorSets(mDevice, &allocInfo, &mVkDescriptorSet) == VK_SUCCESS, "VulkanLayout", "Failed to allocate descriptor set");

		mVkDescriptorPool = pool;
		mVkDescriptorLayout = layout;
	}
}
