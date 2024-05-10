#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsMemoryType : byte
	{
		DeviceLocal,
		HostVisible,
		HostCoherent,
		HostCached
	};

	GENERATE_ENUM_FLAG(GraphicsMemoryType, byte);
}
