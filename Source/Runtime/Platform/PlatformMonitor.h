#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Monitor/Monitor.h>

namespace Hollow
{
	class PlatformMonitor
	{
	public:
		static DArray<Monitor> GetAllMonitors();
		static Monitor GetPrimaryMonitor();

	public:
		PlatformMonitor() = delete;
		~PlatformMonitor() = delete;
	};
}
