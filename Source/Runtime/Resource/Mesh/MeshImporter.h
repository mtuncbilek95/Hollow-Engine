#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>
#include <Runtime/Memory/MemoryBuffer.h>

namespace Hollow
{
	class RUNTIME_API MeshImporter
	{
	public:
		static MeshResourceLayout Import(String path);

		static uint64 CalculateTotalMesh(const MeshResourceLayout& layout);
		static uint64 CalculateIndexSize(const MeshResourceLayout& layout);
		static MemoryBuffer* MeshToMemoryBuffer(const MeshResourceLayout& layout);
		static MemoryBuffer* IndexToMemoryBuffer(const MeshResourceLayout& layout);
	public:
		MeshImporter() = delete;
		~MeshImporter() = delete;
	};
}
