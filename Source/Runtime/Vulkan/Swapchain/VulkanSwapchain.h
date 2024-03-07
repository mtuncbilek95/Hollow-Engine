#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice);
		~VulkanSwapchain() override = default;

		VkSwapchainKHR GetVkSwapchain() const { return mVkSwapchain; }
		VkSurfaceKHR GetVkSurface() const { return mVkSurface; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkDevice GetVkDevice() const { return mVkDevice; }
		VkInstance GetVkInstance() const { return mVkInstance; }

		// Inherited via Swapchain
		void OnShutdown() noexcept override;

	protected:
		virtual void ResizeImpl(const Vector2u& newSize) override;
		virtual void PresentImpl(Semaphore** ppWaitSemaphores, uint32 amount) override;

	private:
		VkSurfaceKHR mVkSurface;
		VkSwapchainKHR mVkSwapchain;
		VkPhysicalDevice mVkPhysicalDevice;
		VkDevice mVkDevice;
		VkInstance mVkInstance;
	};
}