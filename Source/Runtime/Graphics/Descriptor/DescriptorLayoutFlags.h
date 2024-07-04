#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API DescriptorLayoutFlags : u8
	{
		None = 0,
		PushDescriptor = 1,
		UpdateAfterBind = 2,
		HostOnly = 4,
		BufferBit = 10,
		ImmutableSamplers = 20,
		PerStageDescriptor = 40,
		IndirectBindable = 80
	};

	GENERATE_ENUM_FLAG(DescriptorLayoutFlags, u8);
}
