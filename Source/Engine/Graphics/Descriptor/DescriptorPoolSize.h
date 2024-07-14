#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorType.h>

namespace Hollow
{
	struct ENGINE_API DescriptorPoolSize
	{
		DescriptorType Type;
		u64 Count;
	};
}