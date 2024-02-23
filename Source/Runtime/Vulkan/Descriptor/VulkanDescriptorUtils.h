#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>
#include <Runtime/Graphics/Descriptor/DescriptorMemoryType.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorUtils
	{
	public:
		static VkDescriptorType GetVkDescriptorType(DescriptorResourceType type)
		{
			switch (type)
			{
			case DescriptorResourceType::Sampler:
				return VK_DESCRIPTOR_TYPE_SAMPLER;
			case DescriptorResourceType::ConstantBuffer:
				return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case DescriptorResourceType::SampledTexture:
				return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			case DescriptorResourceType::StorageTexture:
				return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case DescriptorResourceType::StorageBuffer:
				return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			default:
				return VK_DESCRIPTOR_TYPE_MAX_ENUM;
			}
		}

	public:
		VulkanDescriptorUtils() = delete;
		~VulkanDescriptorUtils() = delete;
	};
}