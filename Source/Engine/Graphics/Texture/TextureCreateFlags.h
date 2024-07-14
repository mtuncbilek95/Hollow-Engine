#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API TextureCreateFlags : u32
	{
		Undefined = 0,
		SparseBinding = 1 << 0,
		SparseResidency = 1 << 1,
		SparseAliased = 1 << 2,
		MutableFormat = 1 << 3,
		CubeCompatible = 1 << 4,
		Array2D = 1 << 5,
		SplitInstanceBindRegions = 1 << 6,
		BlockTexelView = 1 << 7,
		ExtendedUsage = 1 << 8,
		Disjoint = 1 << 9,
		Alias = 1 << 10,
		Protected = 1 << 11
	};

	GENERATE_ENUM_FLAG(TextureCreateFlags, u32);
}
