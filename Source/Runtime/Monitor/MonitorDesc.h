#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API MonitorDesc
	{
		String MonitorName;
		Vec2u MonitorResolution;
		Vec2i MonitorPosition;
		bool IsPrimary;
	};
}
