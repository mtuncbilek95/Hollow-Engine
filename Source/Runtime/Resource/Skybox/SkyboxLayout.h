#pragma once 

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct SkyboxVertex
	{
		Vec3f Position;
	};

	struct SkyboxLayout
	{
		DArray<SkyboxVertex> Vertices;
		DArray<u32> Indices;
	};

	struct SkyboxResourceLayout
	{
		SkyboxLayout Layout;
	};
}
