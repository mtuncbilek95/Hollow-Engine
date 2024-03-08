#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandPool.h>

namespace Hollow
{
	struct RUNTIME_API CommandBufferDesc
	{
		CommandPool* pOwnerPool;
	};
}