#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsAdapter.h>

#include <vulkan.h>

namespace Hollow
{
	struct RUNTIME_API VulkanAdapter : public GraphicsAdapter
	{
		VkPhysicalDevice PhysicalDevice;
	};
}