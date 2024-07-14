#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Swapchain/PresentMode.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace Utils
	{
		static VkPresentModeKHR GetVkPresentMode(PresentMode mode)
		{
			switch (mode)
			{
			case PresentMode::Immediate:
				return VK_PRESENT_MODE_IMMEDIATE_KHR;
			case PresentMode::VSyncImmediate:
				return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
			case PresentMode::VSyncQueued:
				return VK_PRESENT_MODE_MAILBOX_KHR;
			case PresentMode::FullVSync:
				return VK_PRESENT_MODE_FIFO_KHR;
			default:
				return VK_PRESENT_MODE_FIFO_KHR;
			}
		}
	}
}
