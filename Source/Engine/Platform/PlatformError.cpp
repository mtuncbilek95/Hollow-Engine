#include "PlatformError.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Engine/Win32/Win32Error.h>
typedef Hollow::Win32Error PlatformDependency;
#endif

namespace Hollow
{
	String Hollow::PlatformError::GetPlatformError()
	{
		return PlatformDependency::GetPlatformError();
	}
}
