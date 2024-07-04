#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolSize.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolFlags.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorPoolDesc
	{
		u64 MaxSets;
		DArray<DescriptorPoolSize> PoolSizes;
		DescriptorPoolFlags Flags;
	};
}