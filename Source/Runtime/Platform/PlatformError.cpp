#include "PlatformError.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Error.h>
typedef Hollow::Win32Error PlatformDependency;
#endif

namespace Hollow
{
	string Hollow::PlatformError::GetPlatformError()
	{
		return PlatformDependency::GetPlatformError();
	}
}
