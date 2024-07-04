#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API DescriptorPoolFlags : u8
	{
		None = 0,
		FreeDescriptorSet = 1 << 0,
		UpdateAfterBind = 1 << 1,
		HostOnly = 1 << 2,
		OverallAllocationSets = 1 << 3,
		OverallAllocationPool = 1 << 4
	};

	GENERATE_ENUM_FLAG(DescriptorPoolFlags, u8);
}