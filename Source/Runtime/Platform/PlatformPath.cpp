#include "PlatformPath.h"

#if defined(HOLLOW_PLATFORM_WINDOWS)
#include <Runtime/Win32/Win32Path.h>
typedef Hollow::Win32Path PlatformDependency;
#endif

namespace Hollow
{
	string PlatformPath::GetExecutableDir()
	{
		return PlatformDependency::GetExecutableDir();
	}

	string PlatformPath::GetProgramFilesDir()
	{
		return PlatformDependency::GetProgramFilesDir();
	}

	string PlatformPath::GetAppDataDir()
	{
		return PlatformDependency::GetAppDataDir();
	}

	string PlatformPath::GetTempDir()
	{
		return PlatformDependency::GetTempDir();
	}
}	
