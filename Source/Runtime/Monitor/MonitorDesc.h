#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	struct RUNTIME_API MonitorDesc
	{
		string MonitorName;
		Vector2i MonitorResolution;
		uint16 RefreshRate;
		byte BitsPerPixel;

		arrayList<Vector2i> SupportedResolutions;
	};
}
