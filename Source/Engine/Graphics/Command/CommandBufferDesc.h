#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CommandPool.h>

namespace Hollow
{
	struct ENGINE_API CommandBufferDesc
	{
		SharedPtr<CommandPool> pOwnerPool;
	};
}