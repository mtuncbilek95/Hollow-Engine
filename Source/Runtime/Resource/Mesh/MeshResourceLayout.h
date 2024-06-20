#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API VertexData
	{
		Vector3f Position;
		Vector3f Normal;
		Vector3f Tangent;
		Vector3f Bitangent;
		Vector4f Color;
		Vector2f UV;
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
