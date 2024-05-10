#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetCopyEntry.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetCopyDesc
	{
		arrayList<DescriptorSetCopyEntry> Entries;
	};
}