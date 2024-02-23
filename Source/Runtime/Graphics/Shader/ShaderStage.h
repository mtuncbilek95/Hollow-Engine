#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ShaderStage : byte
	{
		Vertex,
		Pixel,
		TesellationEval,
		TesellationControl,
		Geometry,
		Compute
	};

	GENERATE_ENUM_FLAG(ShaderStage, byte);
}
