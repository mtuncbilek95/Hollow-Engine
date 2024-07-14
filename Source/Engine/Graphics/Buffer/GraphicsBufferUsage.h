#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API GraphicsBufferUsage : u8
	{
		Vertex = 1 << 0,
		Index = 1 << 1,
		Uniform = 1 << 2,
		Storage = 1 << 3,
		TransferSrc = 1 << 4,
		TransferDst = 1 << 5,
	};

	GENERATE_ENUM_FLAG(GraphicsBufferUsage, u8);
}
