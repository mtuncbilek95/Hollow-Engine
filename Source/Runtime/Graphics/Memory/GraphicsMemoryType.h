#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsMemoryType : byte
	{
		Local,
		Host
	};

	GENERATE_ENUM_FLAG(GraphicsMemoryType, byte);
}