#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Core/Guid.h>

namespace Hollow
{
	class RUNTIME_API PlatformGuid final
	{
	public:
		static void GenerateGuid(Guid& guidOut);

	public:
		PlatformGuid() = delete;
		~PlatformGuid() = delete;
	};
}
