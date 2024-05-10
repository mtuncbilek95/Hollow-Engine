#include "VulkanDescriptorPool.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice) : DescriptorPool(desc, pDevice), mVkDescriptorPool(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
		arrayList<VkDescriptorPoolSize> poolSizes;

		uint32 maxSets = 0;

		for (uint8 i = 0; i < desc.PoolSizes.size(); i++)
		{
			VkDescriptorPoolSize poolSize = {};
			poolSize.type = VulkanDescriptorUtils::GetVkDescriptorType(desc.PoolSizes[i].Type);
			poolSize.descriptorCount = desc.PoolSizes[i].Count;
			maxSets = desc.PoolSizes[i].Count;

			poolSizes.push_back(poolSize);
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = maxSets;
		poolInfo.pNext = nullptr;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		CORE_ASSERT(vkCreateDescriptorPool(mVkDevice, &poolInfo, nullptr, &mVkDescriptorPool) == VK_SUCCESS, "VulkanDescriptorPool", "Failed to create descriptor pool!");
	}

	void VulkanDescriptorPool::OnShutdown() noexcept
	{
		if(mVkDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(mVkDevice, mVkDescriptorPool, nullptr);
			mVkDescriptorPool = VK_NULL_HANDLE;
		}

		mVkDevice = VK_NULL_HANDLE;
	}
}