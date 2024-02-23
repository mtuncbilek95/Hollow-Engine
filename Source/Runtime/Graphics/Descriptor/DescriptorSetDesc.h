#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>

namespace Hollow
{
	struct RUNTIME_API DescriptorSetDesc
	{
		SharedPtr<DescriptorLayout> pLayout;
		SharedPtr<DescriptorPool> pPool;
	};
}
