#include "VDescriptorLayout.h"

#include <Engine/Vulkan/Descriptor/VDescriptorUtils.h>
#include <Engine/Vulkan/Shader/VShaderUtils.h>

namespace Hollow
{
	VDescriptorLayout::VDescriptorLayout(const DescriptorLayoutDesc& desc, const SharedPtr<VDevice> pDevice) : DescriptorLayout(desc, pDevice), 
		mVkDescriptorSetLayout(VK_NULL_HANDLE), mDevice(pDevice->GetVkDevice())
	{
		DArray<VkDescriptorSetLayoutBinding> bindings;
		DArray<VkDescriptorBindingFlags> bindingFlags;

		for (const DescriptorLayoutEntry& entry : desc.Entries)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = entry.Binding;
			binding.descriptorType = Utils::GetVkDescriptorType(entry.Type);
			binding.descriptorCount = 1;
			binding.stageFlags = Utils::GetVkShaderStageBit(entry.ShaderStages);
			binding.pImmutableSamplers = nullptr;

			bindings.push_back(binding);

			VkDescriptorBindingFlags bindingFlag = Utils::GetVkDescriptorBindingFlags(entry.Flags);
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
		layoutInfo.flags = Utils::GetVkDescriptorSetLayoutFlags(desc.Flags);
		layoutInfo.pNext = &bindingFlagsInfo;

		CORE_ASSERT(vkCreateDescriptorSetLayout(mDevice, &layoutInfo, nullptr, &mVkDescriptorSetLayout) == VK_SUCCESS, "VDescriptorLayout", "Failed to create descriptor set layout");
	}
}
