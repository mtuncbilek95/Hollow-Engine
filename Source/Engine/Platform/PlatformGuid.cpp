#include "PlatformGuid.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Engine/Win32/Win32Guid.h>
typedef Hollow::Win32Guid PlatformDependency;
#endif

namespace Hollow
{
	void PlatformGuid::GenerateGuid(Guid& guidOut)
	{
		PlatformDependency::GenerateGuid(guidOut);
	}
}
