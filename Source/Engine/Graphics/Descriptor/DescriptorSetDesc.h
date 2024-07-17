#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorPool.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>

namespace Hollow
{
	struct ENGINE_API DescriptorSetDesc
	{
		WeakPtr<DescriptorLayout> pLayout;
		WeakPtr<DescriptorPool> pOwnerPool;
	};
}