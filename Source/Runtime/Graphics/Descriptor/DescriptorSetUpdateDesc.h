#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetUpdateEntry.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetUpdateDesc
	{
		DArray<DescriptorSetUpdateEntry> Entries;
	};
}