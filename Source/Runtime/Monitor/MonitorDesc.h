#pragma once

#include <Runtime/Core/Definitions.h>
#include <Runtime/Core/StdFix.h>

namespace Hollow
{
	struct RUNTIME_API MonitorDesc
	{
		String MonitorName;
		Vector2i MonitorResolution;
		uint16 RefreshRate;
		byte BitsPerPixel;

		ArrayList<Vector2i> SupportedResolutions;
	};
}
