#include "DescriptorLayout.h"

namespace MiniVk
{
	DescriptorLayout::DescriptorLayout(const DescriptorLayoutDesc& desc, Renderer* pRenderer)
	{
		mDevice = pRenderer->GetVkLogicalDevice();

		VkDescriptorSetLayoutBinding bindings[32];

		int i = 0;
		for (const LayoutEntry& entry : desc.LayoutEntries)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = entry.Binding;

			switch (entry.Type)
			{
			case DescriptorType::Sampler:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				break;
			case DescriptorType::CombinedImageSampler:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				break;
			case DescriptorType::SampledImage:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				break;
			case DescriptorType::StorageImage:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				break;
			case DescriptorType::UniformTexelBuffer:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
				break;
			case DescriptorType::StorageTexelBuffer:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
				break;
			case DescriptorType::UniformBuffer:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case DescriptorType::StorageBuffer:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				break;
			case DescriptorType::UniformBufferDynamic:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				break;
			case DescriptorType::StorageBufferDynamic:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
				break;
			case DescriptorType::InputAttachment:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
				break;
			default:
				DEV_LOG(HE_ERROR, "Unknown descriptor type!");
				break;
			}

			binding.descriptorCount = 1;

			switch (entry.Stage)
			{
			case ShaderStage::Vertex:
				binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
				break;
			case ShaderStage::Fragment:
				binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				break;
			case ShaderStage::Geometry:
				binding.stageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;
				break;
			case ShaderStage::TessellationControl:
				binding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
				break;
			case ShaderStage::TessellationEvaluation:
				binding.stageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
				break;
			case ShaderStage::Compute:
				binding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
				break;
			default:
				DEV_LOG(HE_ERROR, "Unknown shader stage!");
				break;
			}

			binding.pImmutableSamplers = nullptr;

			bindings[i] = binding;
			i++;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = desc.LayoutEntries.size();
		layoutInfo.pBindings = bindings;

		DEV_ASSERT(vkCreateDescriptorSetLayout(mDevice, &layoutInfo, nullptr, &mLayout) == VK_SUCCESS, "DescriptorLayout", "Failed to create descriptor set layout!");

	}

	DescriptorLayout::~DescriptorLayout()
	{
		if (mLayout != VK_NULL_HANDLE)
			vkDestroyDescriptorSetLayout(mDevice, mLayout, nullptr);

		mLayout = VK_NULL_HANDLE;
		mDevice = VK_NULL_HANDLE;
	}
}