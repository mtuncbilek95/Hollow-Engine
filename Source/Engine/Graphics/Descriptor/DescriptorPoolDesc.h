#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Memory/GraphicsMemoryType.h>
#include <Engine/Graphics/Descriptor/DescriptorPoolSize.h>
#include <Engine/Graphics/Descriptor/DescriptorPoolFlags.h>

namespace Hollow
{
	struct ENGINE_API DescriptorPoolDesc
	{
		u64 MaxSets;
		DArray<DescriptorPoolSize> PoolSizes;
		DescriptorPoolFlags Flags;
	};
}