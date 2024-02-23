#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorLayout : public DescriptorLayout
	{
	public:
		VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* device);
		~VulkanDescriptorLayout() override = default;

		const VkDescriptorSetLayout& GetVkDescriptorLayout() const { return mVkDescriptorLayout; }

		void OnShutdown() noexcept override;

	private:
		VkDescriptorSetLayout mVkDescriptorLayout;
		VkDevice mVkLogicalDevice;
	};
}