#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Common/GraphicsAPI.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc
	{
		GraphicsAPI API = GraphicsAPI::Vulkan;
		SharedPtr<GraphicsAdapter> Adapter;
		byte GraphicsQueueCount;
		byte ComputeQueueCount;
		byte TransferQueueCount;
	};
}
