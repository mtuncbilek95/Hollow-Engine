#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorSetUpdateEntry.h>

namespace Hollow
{
	struct ENGINE_API DescriptorSetUpdateDesc
	{
		DArray<DescriptorSetUpdateEntry> Entries;
	};
}