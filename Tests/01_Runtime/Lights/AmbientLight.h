#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API AmbientLight
	{
		Vec4f Color;
		alignas(16) f32 Intensity;
	};

	struct RUNTIME_API DiffuseLight
	{
		Vec4f Position;
		Vec4f Color;
		alignas(16) f32 Intensity;
	};

	struct RUNTIME_API SpecularLight
	{
		Vec4f Position;
		Vec4f Color;
		alignas(16) f32 Intensity;
		alignas(16) f32 Shininess;
	};
}
