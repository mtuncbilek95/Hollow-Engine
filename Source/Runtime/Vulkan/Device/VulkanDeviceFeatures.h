#pragma once

#include <Runtime/Core/Core.h>

#include <vulkan.h>

namespace Hollow
{
	struct RUNTIME_API VulkanDeviceFeatures
	{
		VkPhysicalDeviceFeatures features;
	};
}
