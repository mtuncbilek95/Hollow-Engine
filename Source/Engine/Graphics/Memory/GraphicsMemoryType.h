#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API GraphicsMemoryType : u8
	{
		DeviceLocal = 1 << 0,
		HostVisible = 1 << 1,
		HostCoherent = 1 << 2,
		HostCached = 1 << 3
	};

	GENERATE_ENUM_FLAG(GraphicsMemoryType, u8);
}
