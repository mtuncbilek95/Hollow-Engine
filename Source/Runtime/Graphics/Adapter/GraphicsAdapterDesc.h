#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Instance/GraphicsInstance.h>

namespace Hollow
{
	struct RUNTIME_API GraphicsAdapterDesc
	{
		GraphicsAdapterDesc() : ImageDimensions({0, 0, 0}), ProductName("N/A"), VRam(0), pInstance(nullptr) {}

		Vector3u ImageDimensions;
		String ProductName;
		uint32 VRam;
		GraphicsInstance* pInstance;
	};
}
