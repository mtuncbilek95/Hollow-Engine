#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API MeshTopology : byte
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		PatchList,
		TriangleStrip,
		TriangleFan
	};
}