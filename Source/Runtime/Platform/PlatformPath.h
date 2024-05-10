#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API PlatformPath final
	{
	public:
		static String GetExecutableDir();
		static String GetProgramFilesDir();
		static String GetAppDataDir();
		static String GetTempDir();

	public:
		PlatformPath() = delete;
		~PlatformPath() = delete;
	};
}
