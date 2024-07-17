#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CmdPool.h>

namespace Hollow
{
	struct ENGINE_API CmdBufferDesc
	{
		WeakPtr<CmdPool> pOwnerPool;
	};
}