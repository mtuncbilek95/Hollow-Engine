#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>

namespace Hollow
{
	class RUNTIME_API MeshImporter
	{
	public:
		static MeshResourceLayout Import(String path);

		static uint64 CalculateTotalMesh(const MeshResourceLayout& layout);
		static uint64 CalculateTotalSubMesh(const SubMeshObject& subMesh);

	public:
		MeshImporter() = delete;
		~MeshImporter() = delete;
	};
}
