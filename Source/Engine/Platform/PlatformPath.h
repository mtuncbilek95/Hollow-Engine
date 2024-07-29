#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class PlatformPath final
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
