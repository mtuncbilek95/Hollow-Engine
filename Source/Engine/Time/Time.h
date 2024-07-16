#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	struct ENGINE_API Time
	{
		u16 Day;
		u16 Month;
		u16 Year;

		u16 Hour;
		u16 Minute;
		u16 Second;

		u16 Millisecond;
	};
}
