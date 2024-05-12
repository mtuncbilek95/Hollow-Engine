#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	struct RUNTIME_API SubMeshObject
	{
		ArrayList<Vector3f> Positions;
		ArrayList<Vector3f> Normals;
		ArrayList<Vector3f> Tangents;
		ArrayList<Vector3f> Bitangents;
		ArrayList<ArrayList<Vector2f>> UVs;
		ArrayList<ArrayList<Vector4f>> Colors;
		ArrayList<uint32> Indices;
	};
}
