#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutEntry.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorLayoutDesc
	{
		arrayList<DescriptorLayoutEntry> Entries;
	};
}