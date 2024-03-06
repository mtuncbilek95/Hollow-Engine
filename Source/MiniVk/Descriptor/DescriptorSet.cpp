#include "DescriptorSet.h"

#include <Descriptor/DescriptorPool.h>
#include <Descriptor/DescriptorLayout.h>

namespace MiniVk
{
	DescriptorSet::DescriptorSet(const DescriptorSetDesc& desc, Renderer* pRenderer)
		: mDescriptorSet(VK_NULL_HANDLE)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		mOwnerPool = desc.pPool->GetPool();
		mTargetLayout = desc.pLayout->GetLayout();

		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = mOwnerPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &mTargetLayout;

		vkAllocateDescriptorSets(mDevice, &allocInfo, &mDescriptorSet);
	}

	DescriptorSet::~DescriptorSet()
	{
		if (mDescriptorSet != VK_NULL_HANDLE)
			vkFreeDescriptorSets(mDevice, mOwnerPool, 1, &mDescriptorSet);
	}
}