#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Command/CmdPool.h>

namespace Hollow
{
	struct CmdBufferDesc
	{
		WeakPtr<CmdPool> pOwnerPool;
	};
}