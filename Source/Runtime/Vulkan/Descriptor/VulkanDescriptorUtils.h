#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorType.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetFlags.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutFlags.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorUtils
	{
	public:
		static VkDescriptorType GetVkDescriptorType(DescriptorType type)
		{
			switch (type)
			{
			case DescriptorType::Sampler: return VK_DESCRIPTOR_TYPE_SAMPLER;
			case DescriptorType::CombinedImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case DescriptorType::SampledImage: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			case DescriptorType::StorageImage: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case DescriptorType::UniformTexelBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			case DescriptorType::StorageTexelBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
			case DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			case DescriptorType::UniformBufferDynamic: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			case DescriptorType::StorageBufferDynamic: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
			case DescriptorType::InputAttachment: return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
			default: return VK_DESCRIPTOR_TYPE_MAX_ENUM;
			}
		}

		static VkDescriptorBindingFlags GetVkDescriptorBindingFlags(DescriptorSetFlags flags)
		{
			VkDescriptorBindingFlags vkFlags = 0;

			if (flags == DescriptorSetFlags::None)
				return vkFlags;
			if (flags & DescriptorSetFlags::UpdateAfterBind)
				vkFlags |= VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;
			if (flags & DescriptorSetFlags::UpdateUnusedWhilePending)
				vkFlags |= VK_DESCRIPTOR_BINDING_UPDATE_UNUSED_WHILE_PENDING_BIT;
			if (flags & DescriptorSetFlags::PartiallyBound)
				vkFlags |= VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT;
			if (flags & DescriptorSetFlags::VariableDescriptorCount)
				vkFlags |= VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;

			return vkFlags;
		}

		static VkDescriptorSetLayoutCreateFlags GetVkDescriptorSetLayoutFlags(DescriptorLayoutFlags flags)
		{
			VkDescriptorSetLayoutCreateFlags vkFlags = 0;

			if (flags == DescriptorLayoutFlags::None)
				return vkFlags;
			if (flags & DescriptorLayoutFlags::PushDescriptor)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
			if (flags & DescriptorLayoutFlags::UpdateAfterBind)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
			if (flags & DescriptorLayoutFlags::HostOnly)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_HOST_ONLY_POOL_BIT_EXT;
			if (flags & DescriptorLayoutFlags::BufferBit)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
			if (flags & DescriptorLayoutFlags::ImmutableSamplers)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_EMBEDDED_IMMUTABLE_SAMPLERS_BIT_EXT;
			if (flags & DescriptorLayoutFlags::PerStageDescriptor)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_PER_STAGE_BIT_NV;
			if (flags & DescriptorLayoutFlags::IndirectBindable)
				vkFlags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_INDIRECT_BINDABLE_BIT_NV;

			return vkFlags;
		}
	};
}