#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorSet final : public DescriptorSet
	{
	public:
		VulkanDescriptorSet(const DescriptorSetDesc& desc, const SharedPtr<VulkanDevice>& pDevice);
		~VulkanDescriptorSet() override = default;

		VkDescriptorSet GetVkDescriptorSet() const { return mVkDescriptorSet; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkDescriptorSet mVkDescriptorSet;
		VkDescriptorPool mVkDescriptorPool;
		VkDescriptorSetLayout mVkDescriptorLayout;
		VkDevice mVkDevice;
	};
}
