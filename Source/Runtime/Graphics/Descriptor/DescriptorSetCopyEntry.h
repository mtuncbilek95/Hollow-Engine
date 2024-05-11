#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetCopyEntry
	{
		uint32 SourceBinding;
		uint32 SourceArrayElement;

		uint32 DestinationBinding;
		uint32 DestinationArrayElement;

		uint32 DescriptorCount;
	};
}