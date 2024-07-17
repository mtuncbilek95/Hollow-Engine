#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API TextureLayout : u16
	{
		Undefined,
		General,
		ColorAttachment,
		DepthStencilAttachment,
		ShaderReadOnly,
		TransferSource,
		TransferDestination,
		DepthAttachment,
		Present
	};
}
