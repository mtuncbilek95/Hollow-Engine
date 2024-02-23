#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorSet : public DescriptorSet
	{
	public:
		VulkanDescriptorSet(const DescriptorSetDesc& desc, VulkanDevice* pDevice);
		~VulkanDescriptorSet() override = default;

		VkDescriptorSet GetVkDescriptorSet() const { return mVkDescriptorSet; }

		void OnShutdown() noexcept override;

	private:
		VkDescriptorSet mVkDescriptorSet;
		VkDescriptorPool mVkDescriptorPool;
		VkDevice mVkLogicalDevice;
	};
}