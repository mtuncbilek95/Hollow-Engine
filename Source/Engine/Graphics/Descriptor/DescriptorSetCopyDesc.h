#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorSetCopyEntry.h>

namespace Hollow
{
	struct DescriptorSetCopyDesc
	{
		DArray<DescriptorSetCopyEntry> Entries;
	};
}