#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace Hollow
{
	class ENGINE_API PlatformError final
	{
	public:
		static String GetPlatformError();

	public:
		PlatformError() = delete;
		~PlatformError() = delete;
	};
}
