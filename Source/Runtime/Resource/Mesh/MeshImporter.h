#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>
#include <Runtime/Resource/Material/MaterialResourceLayout.h>
#include <Runtime/Memory/MemoryOwnedBuffer.h>

namespace Hollow
{
	class RUNTIME_API MeshImporter
	{
	public:
		static MeshResourceLayout Import(String path);
		static MaterialResourceLayout ImportMaterial(String path, const MeshResourceLayout& meshLayout);
	public:
		MeshImporter() = delete;
		~MeshImporter() = delete;
	};
}
