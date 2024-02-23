#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorPool : public DescriptorPool
	{
	public:
		VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* device);
		~VulkanDescriptorPool() override = default;

		VkDescriptorPool GetVkDescriptorPool() const { return mVkDescriptorPool; }

		void OnShutdown() noexcept override;

	private:
		VkDescriptorPool mVkDescriptorPool;
		VkDevice mVkLogicalDevice;
	};
}