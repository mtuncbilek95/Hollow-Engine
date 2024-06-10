#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsAdapterType.h>

namespace Hollow
{
	/**
	 * @struct GraphicsAdapter
	 * @brief Represents a graphics adapter.
	 */
	struct RUNTIME_API GraphicsAdapter
	{
		GraphicsAdapterType AdapterType;
		u32 AdapterScore;
		String ProductName;
		u32 VRam;
		u32 DeviceId;
	};
}
