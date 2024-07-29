#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ResolveModeFlags : u8
	{
		None,
		SampleZero,
		Average,
		Min,
		Max
	};
}
