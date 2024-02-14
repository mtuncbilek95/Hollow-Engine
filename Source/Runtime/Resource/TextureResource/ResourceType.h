#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class ResourceType : byte
	{
		BaseColor,
		Normal,
		Roughness,
		Metallic,
		AmbientOcclusion,
		Height,
		Emissive,
		Count
	};
}
