#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>
#include <Runtime/Memory/MemoryOwnedBuffer.h>

namespace Hollow
{
	class RUNTIME_API MeshImporter
	{
	public:
		static MeshResourceLayout Import(String path);

		static u64 CalculateTotalMesh(const MeshResourceLayout& layout);
		static u64 CalculateIndexSize(const MeshResourceLayout& layout);
		static MemoryOwnedBuffer MeshToMemoryBuffer(const MeshResourceLayout& layout);
		static MemoryOwnedBuffer IndexToMemoryBuffer(const MeshResourceLayout& layout);
	public:
		MeshImporter() = delete;
		~MeshImporter() = delete;
	};
}
