#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetCopyEntry
	{
		u32 SourceBinding;
		u32 SourceArrayElement;

		u32 DestinationBinding;
		u32 DestinationArrayElement;

		u32 DescriptorCount;
	};
}