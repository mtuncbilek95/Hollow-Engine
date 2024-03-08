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
		VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, VulkanDevice* pDevice);
		~VulkanDescriptorLayout() override = default;

		VkDescriptorSetLayout GetVkLayout() const noexcept { return mVkLayout; }
		VkDevice GetVkDevice() const noexcept { return mVkDevice; }

		// Inherited via DescriptorLayout
		virtual void OnShutdown() noexcept override;

	private:
		VkDescriptorSetLayout mVkLayout;
		VkDevice mVkDevice;
	};
}