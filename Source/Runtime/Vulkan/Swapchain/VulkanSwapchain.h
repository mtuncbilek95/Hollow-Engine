#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Vulkan/Device/VulkanDevice.h>

#include <vulkan.h>

namespace Hollow
{
	// TODO : Remove this forward declaration and implement the VulkanQueue class
	class VulkanQueue;
	class RUNTIME_API VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain(const SwapchainDesc& desc, VulkanDevice* pDevice);
		~VulkanSwapchain() override = default;

		void Free();

		const VkSwapchainKHR& GetVkSwapchain() const noexcept { return mVkSwapchain; }

		// Inherited via Swapchain
		void OnShutdown() noexcept override;

	protected:
		void PresentCore(Semaphore** ppSemaphores, uint32 waitSemaphoreCount) override;
		void ResizeCore(Vector2u newSize) override;
	private:
		VkSurfaceKHR mVkSurface;
		VkSwapchainKHR mVkSwapchain;
		VkPhysicalDevice mVkPhysicalDevice;
		VkDevice mVkLogicalDevice;
		VkInstance mVkInstance;
	};
}
