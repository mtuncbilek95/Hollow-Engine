#include "VulkanDescriptorPool.h"

namespace Hollow
{
	VulkanDescriptorPool::VulkanDescriptorPool(const DescriptorPoolDesc& desc, const SharedPtr<VulkanDevice>& pDevice) : DescriptorPool(desc, pDevice), 
		mVkDescriptorPool(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		ArrayList<VkDescriptorPoolSize> poolSizes;

		uint32 maxSets = 0;

		for (auto& poolSize : desc.PoolSizes)
		{
			VkDescriptorPoolSize size = {};
			size.type = static_cast<VkDescriptorType>(poolSize.Type);
			size.descriptorCount = poolSize.Count;

			poolSizes.push_back(size);

			maxSets += poolSize.Count;
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = maxSets;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateDescriptorPool(mVkDevice, &poolInfo, nullptr, &mVkDescriptorPool) == VK_SUCCESS, "VulkanDescriptorPool", "Failed to create descriptor pool");
	}

	void VulkanDescriptorPool::OnShutdown() noexcept
	{
		if (mVkDescriptorPool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(mVkDevice, mVkDescriptorPool, nullptr);
			mVkDescriptorPool = VK_NULL_HANDLE;
		}

		mVkDevice = VK_NULL_HANDLE;
	}
}
