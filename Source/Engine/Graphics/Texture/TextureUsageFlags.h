#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class TextureUsageFlags : u16
	{
		None = 0,
		SampledImage = 1 << 0,
		StorageImage = 1 << 1,
		ColorAttachment = 1 << 2,
		DepthStencilAttachment = 1 << 3,
		TransientAttachment = 1 << 4,
		InputAttachment = 1 << 5,
		TransferSource = 1 << 6,
		TransferDestination = 1 << 7
	};

	GENERATE_ENUM_FLAG(TextureUsageFlags, u16);
}
