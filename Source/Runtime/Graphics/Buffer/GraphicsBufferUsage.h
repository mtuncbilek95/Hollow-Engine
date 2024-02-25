#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API GraphicsBufferUsage : byte
	{
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer,
		Storage,
		TransferSource,
		TransferDestination,
		IndirectBuffer
	};

	GENERATE_ENUM_FLAG(GraphicsBufferUsage, byte);
}