#include "VulkanDescriptorPool.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice) : DescriptorPool(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		Array<VkDescriptorPoolSize> poolSizes;
		for (byte i = 0; i < desc.PoolSizes.size(); i++)
		{
			VkDescriptorPoolSize poolSize;
			poolSize.type = VulkanDescriptorUtils::GetVkDescriptorType(desc.PoolSizes[i].Type);
			poolSize.descriptorCount = desc.PoolSizes[i].Count;

			poolSizes.push_back(poolSize);
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = desc.SetCount;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		if (desc.MemoryType == DescriptorMemoryType::CPU)
		{
			poolInfo.flags |= VK_DESCRIPTOR_POOL_CREATE_HOST_ONLY_BIT_EXT;
		}
		poolInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateDescriptorPool(mVkLogicalDevice, &poolInfo, nullptr, &mVkDescriptorPool) == VK_SUCCESS, "VulkanDescriptorPool", 
			"Failed to create descriptor pool");
	}

	void VulkanDescriptorPool::OnShutdown() noexcept
	{
		if (mVkDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(mVkLogicalDevice, mVkDescriptorPool, nullptr);
			mVkDescriptorPool = VK_NULL_HANDLE;
		}
	}
}