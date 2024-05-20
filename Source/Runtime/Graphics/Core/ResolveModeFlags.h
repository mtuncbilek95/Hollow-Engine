#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ResolveModeFlags : byte
	{
		None,
		SampleZero,
		Average,
		Min,
		Max
	};

	GENERATE_ENUM_FLAG(ResolveModeFlags, byte);
}
