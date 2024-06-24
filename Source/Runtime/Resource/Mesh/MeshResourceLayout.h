#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API VertexData
	{
		Vec3f Position;
		Vec3f Normal;
		Vec3f Tangent;
		Vec3f Bitangent;
		Vec4f Color;
		Vec2f UV;
	};

	struct RUNTIME_API SubMeshLayout
	{
		DArray<VertexData> Vertices;
		DArray<u32> Indices;
		u32 MeshIndex;
		u32 MaterialIndex;
	};

	struct RUNTIME_API MeshResourceLayout
	{
		DArray<SubMeshLayout> SubMeshes;
	};
}
