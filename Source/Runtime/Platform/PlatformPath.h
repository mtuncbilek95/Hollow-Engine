#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API PlatformPath final
	{
	public:
		static string GetExecutableDir();
		static string GetProgramFilesDir();
		static string GetAppDataDir();
		static string GetTempDir();

	public:
		PlatformPath() = delete;
		~PlatformPath() = delete;
	};
}
