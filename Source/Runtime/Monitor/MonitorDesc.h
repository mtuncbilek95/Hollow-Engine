#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API MonitorDesc
	{
		String MonitorName;
		Vector2u MonitorResolution;
		Vector2i MonitorPosition;
		bool IsPrimary;
	};
}
