#include "VulkanDescriptorLayout.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, const SharedPtr<VulkanDevice> pDevice) : DescriptorLayout(desc, pDevice), 
		mVkDescriptorSetLayout(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		DArray<VkDescriptorSetLayoutBinding> bindings;
		DArray<VkDescriptorBindingFlags> bindingFlags;

		for (const DescriptorLayoutEntry& entry : desc.Entries)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = entry.Binding;
			binding.descriptorType = VulkanDescriptorUtils::GetVkDescriptorType(entry.Type);
			binding.descriptorCount = 1;
			binding.stageFlags = VulkanShaderUtils::GetVkShaderStageBit(entry.ShaderStages);
			binding.pImmutableSamplers = nullptr;

			bindings.push_back(binding);

			VkDescriptorBindingFlags bindingFlag = VulkanDescriptorUtils::GetVkDescriptorBindingFlags(entry.Flags);
			bindingFlags.push_back(bindingFlag);
		}

		VkDescriptorSetLayoutBindingFlagsCreateInfo bindingFlagsInfo = {};
		bindingFlagsInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		bindingFlagsInfo.bindingCount = static_cast<uint32_t>(bindingFlags.size());
		bindingFlagsInfo.pBindingFlags = bindingFlags.data();
		bindingFlagsInfo.pNext = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();
		layoutInfo.flags = VulkanDescriptorUtils::GetVkDescriptorSetLayoutFlags(desc.Flags);
		layoutInfo.pNext = &bindingFlagsInfo;

		CORE_ASSERT(vkCreateDescriptorSetLayout(mVkDevice, &layoutInfo, nullptr, &mVkDescriptorSetLayout) == VK_SUCCESS, "VulkanDescriptorLayout", "Failed to create descriptor set layout");
	}

	VulkanDescriptorLayout::~VulkanDescriptorLayout()
	{
		if (mVkDescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(mVkDevice, mVkDescriptorSetLayout, nullptr);
			mVkDescriptorSetLayout = VK_NULL_HANDLE;
		}

		mVkDevice = nullptr; 
	}
}
