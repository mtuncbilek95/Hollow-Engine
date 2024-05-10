#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#if defined(HOLLOW_PLATFORM_WINDOWS)

namespace Hollow
{
	class RUNTIME_API Win32Path final
	{
	public:
		static String GetExecutableDir();
		static String GetProgramFilesDir();
		static String GetAppDataDir();
		static String GetTempDir();

	public:
		Win32Path() = delete;
		~Win32Path() = delete;
	};
}

#endif