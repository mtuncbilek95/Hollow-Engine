#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorMemoryType.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolSize.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorPoolDesc
	{
		DescriptorMemoryType MemoryType;
		Array<DescriptorPoolSize> PoolSizes;
		uint64 SetCount;
	};
}
