#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API PipelineBindPoint : u8
	{
		Graphics,
		Compute,
		RayTracing
	};
}