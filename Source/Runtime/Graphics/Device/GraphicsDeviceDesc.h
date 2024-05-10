#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsAPI.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc
	{
		GraphicsAPI API = GraphicsAPI::Vulkan;
		GraphicsInstance* Instance;
		byte GraphicsQueueCount;
		byte ComputeQueueCount;
		byte TransferQueueCount;
	};
}
