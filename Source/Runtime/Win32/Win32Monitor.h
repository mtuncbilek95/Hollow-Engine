#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

#include <Runtime/Monitor/Monitor.h>

namespace Hollow
{
	class RUNTIME_API Win32Monitor final
	{
	public:
		static SharedPtr<Monitor> GetPrimaryMonitor();
		static ArrayList<SharedPtr<Monitor>> GetAllMonitors();

	public:
		Win32Monitor() = delete;
		~Win32Monitor() = delete;
	};
}
