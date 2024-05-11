#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Command/CommandPool.h>

namespace Hollow
{
	/**
	 * @class CommandBufferDesc
	 * @brief Describes a command buffer
	 */
	struct RUNTIME_API CommandBufferDesc
	{
		CommandPool* pOwnerPool;
	};
}