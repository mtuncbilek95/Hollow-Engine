#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommandPool final : public CommandPool
	{
	public:
		VulkanCommandPool(const CommandPoolDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanCommandPool() override;

		VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }
		VkDevice GetVkDevice() const { return mVkDevice; }

	private:
		VkCommandPool mVkCommandPool;
		VkDevice mVkDevice;
	};
}
