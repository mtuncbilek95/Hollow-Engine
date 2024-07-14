#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VDevice;
	class ENGINE_API VSwapchain : public Swapchain
	{
	public:
		VSwapchain(const SwapchainDesc& desc, SharedPtr<VDevice> pDevice);
		~VSwapchain() override = default;

		VkSwapchainKHR GetSwapchain() const { return mSwapchain; }
		VkSurfaceKHR GetSurfaceFormat() const { return mSurface; }

	private:
		VkSwapchainKHR mSwapchain;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;
		VkPhysicalDevice mAdapter;
		VkInstance mInstance;
	};
}
