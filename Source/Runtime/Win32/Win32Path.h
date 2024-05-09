#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#if defined(HOLLOW_PLATFORM_WINDOWS)

namespace Hollow
{
	class RUNTIME_API Win32Path final
	{
	public:
		static string GetExecutableDir();
		static string GetProgramFilesDir();
		static string GetAppDataDir();
		static string GetTempDir();

	public:
		Win32Path() = delete;
		~Win32Path() = delete;
	};
}

#endif