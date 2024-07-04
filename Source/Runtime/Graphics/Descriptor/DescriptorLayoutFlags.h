#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API DescriptorLayoutFlags : u8
	{
        None = 0,
        PushDescriptor = 1 << 0,
        UpdateAfterBind = 1 << 1,
        HostOnly = 1 << 2,
        BufferBit = 1 << 3,
        ImmutableSamplers = 1 << 4,
        PerStageDescriptor = 1 << 5,
        IndirectBindable = 1 << 6,
	};

	GENERATE_ENUM_FLAG(DescriptorLayoutFlags, u8);
}
