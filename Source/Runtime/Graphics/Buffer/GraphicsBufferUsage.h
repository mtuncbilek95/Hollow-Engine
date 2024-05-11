#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	/**
	 * @enum GraphicsBufferUsage
	 * @brief Enum class GraphicsBufferUsage
	 */
	enum class RUNTIME_API GraphicsBufferUsage : byte
	{
		Vertex = 1,
		Index = 2,
		Uniform = 4,
		Storage = 8,
		TransferSource = 16,
		TransferDestination = 32
	};

	GENERATE_ENUM_FLAG(GraphicsBufferUsage, byte);
}