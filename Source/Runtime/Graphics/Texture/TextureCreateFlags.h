#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class TextureCreateFlags : u16
	{
		None = 0,
		SparseBinding = 1,
		SparseResidency = 2,
		SparseAliased = 4,
		MutableFormat = 8,
		CubeMap = 16,
		Array2D = 32,
		SplitInstanceBindRegions = 64,
		BlockTexelView = 128,
		ExtendedUsage = 256,
		Disjoint = 512,
		Alias = 1024,
		Protected = 2048
	};

	GENERATE_ENUM_FLAG(TextureCreateFlags, u16);
}
