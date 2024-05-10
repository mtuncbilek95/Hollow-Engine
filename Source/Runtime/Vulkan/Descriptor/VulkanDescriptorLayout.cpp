#include "VulkanDescriptorLayout.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* pDevice) : DescriptorLayout(desc, pDevice), mVkLayout(VK_NULL_HANDLE),
		mVkDevice(pDevice->GetVkDevice())
	{
		arrayList<VkDescriptorSetLayoutBinding> bindings;

		for (uint8 i = 0; i < desc.Entries.size(); i++)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = desc.Entries[i].Binding;
			binding.descriptorType = VulkanDescriptorUtils::GetVkDescriptorType(desc.Entries[i].Type);
			binding.descriptorCount = 1;
			binding.stageFlags = VulkanShaderUtils::GetVkShaderStageBit(desc.Entries[i].ShaderStages);
			binding.pImmutableSamplers = nullptr;

			bindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = bindings.size();
		layoutInfo.pBindings = bindings.data();
		layoutInfo.flags = VkDescriptorSetLayoutCreateFlags();
		layoutInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateDescriptorSetLayout(mVkDevice, &layoutInfo, nullptr, &mVkLayout) == VK_SUCCESS, "VulkanLayout", "Failed to create descriptor set layout!");
	}

	void VulkanDescriptorLayout::OnShutdown() noexcept
	{
		if(mVkLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(mVkDevice, mVkLayout, nullptr);
			mVkLayout = VK_NULL_HANDLE;
		}

		mVkDevice = nullptr;
	}
}