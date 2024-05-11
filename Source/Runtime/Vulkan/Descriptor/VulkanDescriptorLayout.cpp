#include "VulkanDescriptorLayout.h"

#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>
#include <Runtime/Vulkan/Shader/VulkanShaderUtils.h>

namespace Hollow
{
	VulkanDescriptorLayout::VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, const SharedPtr<VulkanDevice> pDevice) : DescriptorLayout(desc, pDevice), 
		mVkDescriptorSetLayout(VK_NULL_HANDLE), mVkDevice(pDevice->GetVkDevice())
	{
		ArrayList<VkDescriptorSetLayoutBinding> bindings;

		for (const DescriptorLayoutEntry& entry : desc.Entries)
		{
			VkDescriptorSetLayoutBinding binding = {};
			binding.binding = entry.Binding;
			binding.descriptorType = VulkanDescriptorUtils::GetVkDescriptorType(entry.Type);
			binding.descriptorCount = 1;
			binding.stageFlags = VulkanShaderUtils::GetVkShaderStageBit(entry.ShaderStages);
			binding.pImmutableSamplers = nullptr;

			bindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();
		layoutInfo.flags = 0;
		layoutInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateDescriptorSetLayout(mVkDevice, &layoutInfo, nullptr, &mVkDescriptorSetLayout) == VK_SUCCESS, "VulkanDescriptorLayout", "Failed to create descriptor set layout");
	}

	void VulkanDescriptorLayout::OnShutdown() noexcept
	{
		if (mVkDescriptorSetLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(mVkDevice, mVkDescriptorSetLayout, nullptr);
			mVkDescriptorSetLayout = VK_NULL_HANDLE;
		}

		mVkDevice = nullptr;
	}
}
