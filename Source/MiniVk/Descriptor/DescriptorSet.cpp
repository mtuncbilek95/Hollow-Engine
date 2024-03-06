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

		VkDescriptorSetAllocateInfo allocInfos[2];

		for (uint32 i = 0; i < 2; i++)
		{
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = mOwnerPool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &mTargetLayout;

			allocInfos[i] = allocInfo;
		}

		vkAllocateDescriptorSets(mDevice, allocInfos, &mDescriptorSet);
	}

	DescriptorSet::~DescriptorSet()
	{
		if (mDescriptorSet != VK_NULL_HANDLE)
			vkFreeDescriptorSets(mDevice, mOwnerPool, 1, &mDescriptorSet);
	}
}