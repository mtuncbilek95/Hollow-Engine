#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsAdapterType.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsAdapter
	{
		GraphicsAdapterType AdapterType;
		uint32 AdapterScore;
		string ProductName;
		uint32 VRam;
		uint32 DeviceId;
	};
}