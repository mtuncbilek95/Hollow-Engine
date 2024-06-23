#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSwapchain final : public Swapchain
	{
	public:
		VulkanSwapchain(const SwapchainDesc& desc, SharedPtr<VulkanDevice> device);
		~VulkanSwapchain() override;

		VkSwapchainKHR GetVkSwapchain() const { return mVkSwapchain; }
		VkSurfaceKHR GetVkSurface() const { return mVkSurface; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkDevice GetVkDevice() const { return mVkDevice; }
		VkInstance GetVkInstance() const { return mVkInstance; }

	protected:
		virtual void ResizeImpl(const Vector2u& newSize) override;
		virtual void AcquireNextImageImpl(SharedPtr<Fence> fence) override;
		virtual void PresentImpl() override;

	private:
		VkSurfaceKHR mVkSurface;
		VkSwapchainKHR mVkSwapchain;
		VkPhysicalDevice mVkPhysicalDevice;
		VkDevice mVkDevice;
		VkInstance mVkInstance;
	};
}
