#pragma once 

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorLayout final : public DescriptorLayout
	{
	public:
		VulkanDescriptorLayout(const DescriptorLayoutDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanDescriptorLayout() override;

		VkDescriptorSetLayout GetVkDescriptorLayout() const { return mVkDescriptorSetLayout; }
		VkDevice GetVkDevice() const { return mVkDevice; }

	private:
		VkDescriptorSetLayout mVkDescriptorSetLayout;
		VkDevice mVkDevice;
	};
}
