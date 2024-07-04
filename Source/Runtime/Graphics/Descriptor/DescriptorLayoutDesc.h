#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutEntry.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutFlags.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorLayoutDesc
	{
		DArray<DescriptorLayoutEntry> Entries;
		DescriptorLayoutFlags Flags;
	};
}