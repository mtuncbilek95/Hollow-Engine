#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API FaceCullMode : byte
	{
		None = 1,
		Front = 2,
		Back = 4,
		All = Front | Back
	};
}