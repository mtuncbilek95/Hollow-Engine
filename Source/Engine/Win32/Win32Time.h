#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Time/Time.h>

namespace Hollow
{
	class ENGINE_API Win32Time final
	{
	public:
		static Time CurrentDateTime();
	};
}
