#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Common/PresentMode.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSwapchainUtils
	{
	public:
		static VkPresentModeKHR GetVkPresentMode(PresentMode mode)
		{
			switch (mode)
			{
			case PresentMode::Immediate: // VSync Off
				return VK_PRESENT_MODE_IMMEDIATE_KHR;
			case PresentMode::VSyncImmediate: // VSync On, but Tears are allowed
				return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
			case PresentMode::VSyncQueued: // VSync On, but subsequent images are queued
				return VK_PRESENT_MODE_MAILBOX_KHR;
			case PresentMode::FullVSync: // VSync On
				return VK_PRESENT_MODE_FIFO_KHR;
			default:
				return VK_PRESENT_MODE_FIFO_KHR;
			}
		}

	public:
		VulkanSwapchainUtils() = delete;
		~VulkanSwapchainUtils() = delete;
	};
}