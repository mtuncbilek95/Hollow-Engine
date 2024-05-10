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

		VkDescriptorSet GetVkDescriptorSet() const noexcept { return mVkDescriptorSet; }
		VkDevice GetVkDevice() const noexcept { return mVkDevice; }

		// Inherited via DescriptorSet
		virtual void OnShutdown() noexcept override;

	private:
		VkDescriptorSet mVkDescriptorSet;
		VkDescriptorPool mVkDescriptorPool;
		VkDescriptorSetLayout mVkDescriptorLayout;
		VkDevice mVkDevice;
	};
}