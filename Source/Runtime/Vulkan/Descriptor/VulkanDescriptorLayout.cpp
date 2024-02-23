#include "VulkanDescriptorLayout.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* pDevice) : DescriptorLayout(desc, pDevice)
	{
		mVkLogicalDevice = pDevice->GetVkLogicalDevice();

		Array<VkDescriptorSetLayoutBinding> vkLayoutBindings;
		for (byte i = 0; i < desc.Entries.size(); i++)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = desc.Entries[i].Binding;
			binding.descriptorCount = 1;
			binding.descriptorType = VulkanDescriptorUtils::GetVkDescriptorType(desc.Entries[i].Type);
			binding.pImmutableSamplers = nullptr;
			binding.stageFlags = VulkanShaderUtils::GetVkShaderStage(desc.Entries[i].Stage);

			vkLayoutBindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = vkLayoutBindings.size();
		layoutInfo.pBindings = vkLayoutBindings.data();
		layoutInfo.flags = VkDescriptorSetLayoutCreateFlags();
		layoutInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateDescriptorSetLayout(mVkLogicalDevice, &layoutInfo, nullptr, &mVkDescriptorLayout) == VK_SUCCESS, "VulkanDescriptorLayout", 
			"Failed to create descriptor set layout!");
	}

	void VulkanDescriptorLayout::OnShutdown() noexcept
	{
		if(mVkDescriptorLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(mVkLogicalDevice, mVkDescriptorLayout, nullptr);
			mVkDescriptorLayout = VK_NULL_HANDLE;
		}
	}
}