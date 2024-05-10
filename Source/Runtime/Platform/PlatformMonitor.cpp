#include "PlatformMonitor.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Monitor.h>
typedef Hollow::Win32Monitor PlatformDependency;
#endif

namespace Hollow
{
	SharedPtr<Monitor> PlatformMonitor::GetPrimaryMonitor()
	{
		return PlatformDependency::GetPrimaryMonitor();
	}

	ArrayList<SharedPtr<Monitor>> PlatformMonitor::GetAllMonitors()
	{
		return PlatformDependency::GetAllMonitors();
	}
}
