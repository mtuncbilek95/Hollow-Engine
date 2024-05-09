#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API Win32Error final
	{
	public:
		static string GetPlatformError();

	public:
		Win32Error() = delete;
		~Win32Error() = delete;
	};
}
