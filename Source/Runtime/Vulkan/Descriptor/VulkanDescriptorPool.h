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
		VulkanDescriptorPool(const DescriptorPoolDesc& desc, VulkanDevice* pDevice);
		~VulkanDescriptorPool() override = default;

		VkDescriptorPool GetVkDescriptorPool() const noexcept { return mVkDescriptorPool; }
		VkDevice GetVkDevice() const noexcept { return mVkDevice; }

		// Inherited via DescriptorSet
		virtual void OnShutdown() noexcept override;

	private:
		VkDescriptorPool mVkDescriptorPool;
		VkDevice mVkDevice;
	};
}