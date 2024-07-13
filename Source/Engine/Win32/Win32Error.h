#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class ENGINE_API Win32Error final
	{
	public:
		static String GetPlatformError();

	public:
		Win32Error() = delete;
		~Win32Error() = delete;
	};
}
