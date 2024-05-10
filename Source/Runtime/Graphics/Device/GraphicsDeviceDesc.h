#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Core/GraphicsAPI.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc
	{
		SharedPtr<GraphicsInstance> Instance;
		byte GraphicsQueueCount;
		byte ComputeQueueCount;
		byte TransferQueueCount;
	};
}
