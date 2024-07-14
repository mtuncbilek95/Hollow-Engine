#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorSetCopyEntry.h>

namespace Hollow
{
	struct ENGINE_API DescriptorSetCopyDesc
	{
		DArray<DescriptorSetCopyEntry> Entries;
	};
}