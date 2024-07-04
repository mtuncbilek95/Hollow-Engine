#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API DescriptorSetFlags : u8
	{
		None = 0,
		UpdateAfterBind = 1,
		UpdateUnusedWhilePending = 2,
		PartiallyBound = 4,
		VariableDescriptorCount = 8
	};

	GENERATE_ENUM_FLAG(DescriptorSetFlags, u8);
}
