#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ShaderStage : byte
	{
		Vertex,
		Fragment,
		TesellationEval,
		TesellationControl,
		Geometry,
		Compute
	};

	GENERATE_ENUM_FLAG(ShaderStage, byte);
}