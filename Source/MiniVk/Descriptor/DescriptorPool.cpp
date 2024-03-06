#include "DescriptorPool.h"

namespace MiniVk
{
	DescriptorPool::DescriptorPool(const DescriptorPoolDesc& desc, Renderer* pRenderer)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		VkDescriptorPoolSize sizes[16];

		for (uint32 i = 0; i < desc.DescriptorEntries.size(); ++i)
		{
			switch (desc.DescriptorEntries[i].Type)
			{
			case DescriptorType::Sampler:
				sizes[i].type = VK_DESCRIPTOR_TYPE_SAMPLER;
				break;
			case DescriptorType::CombinedImageSampler:
				sizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			case DescriptorType::SampledImage:
				sizes[i].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				break;
			case DescriptorType::StorageImage:
				sizes[i].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				break;
			case DescriptorType::UniformTexelBuffer:
				sizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
				break;
			case DescriptorType::StorageTexelBuffer:
				sizes[i].type = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
				break;
			case DescriptorType::UniformBuffer:
				sizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case DescriptorType::StorageBuffer:
				sizes[i].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				break;
			case DescriptorType::UniformBufferDynamic:
				sizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				break;
			case DescriptorType::StorageBufferDynamic:
				sizes[i].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
				break;
			case DescriptorType::InputAttachment:
				sizes[i].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
				break;
			default:
				DEV_LOG(HE_ERROR, "Unknown descriptor type!");
				break;
			}

			sizes[i].descriptorCount = desc.DescriptorEntries[i].Count;
		}

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = desc.DescriptorEntries.size();
		poolInfo.pPoolSizes = sizes;
		poolInfo.maxSets = desc.MaxSets;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		DEV_ASSERT(vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool) == VK_SUCCESS, "DescriptorPool", "Failed to create descriptor pool!");
	}

	DescriptorPool::~DescriptorPool()
	{
		if (mDescriptorPool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);

		mDescriptorPool = VK_NULL_HANDLE;
		mDevice = VK_NULL_HANDLE;
	}
}