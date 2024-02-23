#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsAdapterDesc
	{
		Vector3u ImageDimensions;
		String ProductName;
		uint32 VRam;
		GraphicsInstance* pInstance;
	};
}
