#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanCommandPool : public CommandPool
	{
	public:
		VulkanCommandPool(const CommandPoolDesc& desc, VulkanDevice* pDevice);
		~VulkanCommandPool() override = default;

		VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }

		void OnShutdown() noexcept override;

	private:
		VkCommandPool mVkCommandPool;
		VkDevice mVkLogicalDevice;
	};
}