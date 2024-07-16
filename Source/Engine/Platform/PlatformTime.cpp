#include "PlatformTime.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Engine/Win32/Win32Time.h>
typedef Hollow::Win32Time PlatformDependency;
#endif

namespace Hollow
{
	Time PlatformTime::CurrentDateTime()
	{
		return PlatformDependency::CurrentDateTime();
	}
}
