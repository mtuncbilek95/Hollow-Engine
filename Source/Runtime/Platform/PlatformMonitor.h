#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>
#include <Runtime/Monitor/Monitor.h>

namespace Hollow
{
	class RUNTIME_API PlatformMonitor final
	{
	public:
		static sharedPtr<Monitor> GetPrimaryMonitor();
		static arrayList<sharedPtr<Monitor>> GetAllMonitors();

	public:
		PlatformMonitor() = delete;
		~PlatformMonitor() = delete;
	};
}
