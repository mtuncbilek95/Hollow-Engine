#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanFence final : public Fence
	{
	public:
		VulkanFence(const FenceDesc& desc, const SharedPtr<VulkanDevice> pDevice);
		~VulkanFence() override = default;

		VkFence GetVkFence() { return mVkFence; }
		VkDevice GetVkDevice() { return mVkDevice; }

		void OnShutdown() noexcept override;

	private:
		VkFence mVkFence;
		VkDevice mVkDevice;
	};
}
