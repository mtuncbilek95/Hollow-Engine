#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ShaderType : byte
	{
		Vertex,
		Pixel,
		Geometry,
		TessellationControl,
		TessellationEvaluation,
		Compute
	};
}
