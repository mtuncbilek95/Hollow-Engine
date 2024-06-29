#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API Transform
	{
		Vec3f Position = Vec3f(0.0f);
		Vec3f Rotation = Vec3f(0.0f);
		Vec3f Scale = Vec3f(1.0f);
	};
}
