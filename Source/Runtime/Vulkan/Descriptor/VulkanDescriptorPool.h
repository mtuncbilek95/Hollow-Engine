#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDescriptorPool final : public DescriptorPool
	{
	public:
		VulkanDescriptorPool(const DescriptorPoolDesc& desc, const SharedPtr<VulkanDevice>& pDevice);
		~VulkanDescriptorPool() override = default;

		VkDescriptorPool GetVkDescriptorPool() const { return mVkDescriptorPool; }
		VkDevice GetVkDevice() const { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkDescriptorPool mVkDescriptorPool;
		VkDevice mVkDevice;
	};
}
