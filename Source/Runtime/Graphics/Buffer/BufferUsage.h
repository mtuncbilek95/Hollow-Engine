#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API BufferUsage : byte
	{
		Unkown,
		VertexBuffer,
		IndexBuffer,
		ConstantBuffer
	};
}
