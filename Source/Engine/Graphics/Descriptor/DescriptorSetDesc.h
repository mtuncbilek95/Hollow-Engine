#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorPool.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>

namespace Hollow
{
	struct ENGINE_API DescriptorSetDesc
	{
		SharedPtr<DescriptorLayout> pLayout;
		SharedPtr<DescriptorPool> pOwnerPool;
	};
}