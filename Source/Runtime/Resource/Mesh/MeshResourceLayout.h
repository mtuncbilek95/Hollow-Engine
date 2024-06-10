#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct VertexData
	{
		ArrayList<Vector3f> Position;
		ArrayList<Vector3f> Normal;
		ArrayList<Vector3f> Tangent;
		ArrayList<Vector3f> Bitangent;
		ArrayList<Vector2f> UV;
		ArrayList<Vector4f> Color;
	};

	struct RUNTIME_API MeshResourceLayout
	{
		VertexData Vertices;
		ArrayList<uint32> Indices;
	};
}
