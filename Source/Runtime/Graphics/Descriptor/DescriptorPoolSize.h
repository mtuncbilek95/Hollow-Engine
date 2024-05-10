#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorType.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorPoolSize
	{
		DescriptorType Type;
		uint64 Count;
	};
}