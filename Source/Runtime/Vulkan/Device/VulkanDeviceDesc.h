#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Vulkan/Device/VulkanDeviceFeatures.h>

namespace Hollow
{
	struct RUNTIME_API VulkanDeviceDesc : public GraphicsDeviceDesc
	{
		Array<const char*> deviceExtensions;
		VulkanDeviceFeatures deviceFeatures;
	};
}
