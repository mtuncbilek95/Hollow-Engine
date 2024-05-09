#include "PlatformMonitor.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Monitor.h>
typedef Hollow::Win32Monitor PlatformDependency;
#endif

namespace Hollow
{
	sharedPtr<Monitor> PlatformMonitor::GetPrimaryMonitor()
	{
		return PlatformDependency::GetPrimaryMonitor();
	}

	arrayList<sharedPtr<Monitor>> PlatformMonitor::GetAllMonitors()
	{
		return PlatformDependency::GetAllMonitors();
	}
}
