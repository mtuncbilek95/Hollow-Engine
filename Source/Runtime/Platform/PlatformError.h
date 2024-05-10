#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	class RUNTIME_API PlatformError final
	{
	public:
		static String GetPlatformError();

	public:
		PlatformError() = delete;
		~PlatformError() = delete;
	};
}
