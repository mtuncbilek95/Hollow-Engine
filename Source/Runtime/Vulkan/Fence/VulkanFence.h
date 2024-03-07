#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanFence : public Fence
	{
	public:
		VulkanFence(const FenceDesc& desc, VulkanDevice* pDevice);
		~VulkanFence() override = default;

		VkFence GetVkFence() { return mVkFence; }
		VkDevice GetVkDevice() { return mVkDevice; }

		// Inherited via Swapchain
		void OnShutdown() noexcept override;

	private:
		VkFence mVkFence;
		VkDevice mVkDevice;
	};
}