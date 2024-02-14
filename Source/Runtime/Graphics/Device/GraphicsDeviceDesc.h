#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsAPI.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapter.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsDeviceDesc
	{
		GraphicsAPI API;
		SharedPtr<GraphicsAdapter> Adapter;
	};
}
