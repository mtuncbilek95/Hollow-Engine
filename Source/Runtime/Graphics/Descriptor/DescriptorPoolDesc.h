#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryType.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolSize.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorPoolDesc
	{
		uint64 MaxSets;
		ArrayList<DescriptorPoolSize> PoolSizes;
	};
}