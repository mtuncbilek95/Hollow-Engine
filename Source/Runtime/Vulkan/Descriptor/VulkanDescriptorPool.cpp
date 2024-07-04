#include "VulkanDescriptorPool.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, const SharedPtr<VulkanDevice>& pDevice) : DescriptorPool(desc, pDevice), 
		mVkDescriptorPool(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		DArray<VkDescriptorPoolSize> poolSizes;

		for (auto& poolSize : desc.PoolSizes)
		{
			VkDescriptorPoolSize size = {};
			size.type = static_cast<VkDescriptorType>(poolSize.Type);
			size.descriptorCount = poolSize.Count;

			poolSizes.push_back(size);
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = desc.MaxSets;
		poolInfo.flags = VulkanDescriptorUtils::GetVkDescriptorPoolFlags(desc.Flags);
		poolInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateDescriptorPool(mVkDevice, &poolInfo, nullptr, &mVkDescriptorPool) == VK_SUCCESS, "VulkanDescriptorPool", "Failed to create descriptor pool");
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (mVkDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(mVkDevice, mVkDescriptorPool, nullptr);
			mVkDescriptorPool = VK_NULL_HANDLE;
		}

		mVkDevice = VK_NULL_HANDLE;
	}
}
