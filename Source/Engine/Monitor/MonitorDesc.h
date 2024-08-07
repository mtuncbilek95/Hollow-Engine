#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	struct MonitorDesc
	{
		String MonitorName;
		Vec2u MonitorResolution;
		Vec2i MonitorPosition;
		bool IsPrimary;
	};
}
