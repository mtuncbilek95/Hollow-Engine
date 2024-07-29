#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class PipelineBindPoint : u8
	{
		Graphics,
		Compute,
		RayTracing
	};
}