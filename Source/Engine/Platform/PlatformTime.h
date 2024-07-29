#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Time/Time.h>

namespace Hollow
{
	class PlatformTime final
	{
	public:
		static Time CurrentDateTime();
	};
}
