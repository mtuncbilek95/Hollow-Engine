#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VDevice;
	class VSwapchain : public Swapchain
	{
	public:
		VSwapchain(const SwapchainDesc& desc, SharedPtr<VDevice> pDevice);
		~VSwapchain() override = default;

		VkSwapchainKHR GetSwapchain() const { return mSwapchain; }
		VkSurfaceKHR GetSurfaceFormat() const { return mSurface; }

	protected:
		virtual u32 AcquireNextImageImpl(WeakPtr<Fence> pFence, WeakPtr<Semaphore> pSemaphore) override;
		virtual void PresentImpl(WeakPtr<Semaphore> pSemaphore) override;

	private:
		VkSwapchainKHR mSwapchain;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;
		VkPhysicalDevice mAdapter;
		VkInstance mInstance;
	};
}
