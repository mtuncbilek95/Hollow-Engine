#include "PlatformPath.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Path.h>
typedef Hollow::Win32Path PlatformDependency;
#endif

namespace Hollow
{
	String PlatformPath::GetExecutableDir()
	{
		return PlatformDependency::GetExecutableDir();
	}

	String PlatformPath::GetProgramFilesDir()
	{
		return PlatformDependency::GetProgramFilesDir();
	}

	String PlatformPath::GetAppDataDir()
	{
		return PlatformDependency::GetAppDataDir();
	}

	String PlatformPath::GetTempDir()
	{
		return PlatformDependency::GetTempDir();
	}
}	
