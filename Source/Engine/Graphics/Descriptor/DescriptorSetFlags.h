#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API DescriptorSetFlags : u8
	{
		Undefined = 0,
		UpdateAfterBind = 1 << 0,
		UpdateUnusedWhilePending = 1 << 1,
		PartiallyBound = 1 << 2,
		VariableDescriptorCount = 1 << 3
	};

	GENERATE_ENUM_FLAG(DescriptorSetFlags, u8);
}
