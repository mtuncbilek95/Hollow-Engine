#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorResourceType.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorPoolSize
	{
		DescriptorResourceType Type;
		uint64 Count;
	};
}