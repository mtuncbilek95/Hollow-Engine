#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>

namespace Hollow
{
	class RUNTIME_API MeshImporter
	{
	public:
		static MeshResourceLayout Import(String& path);

	public:
		MeshImporter() = delete;
		~MeshImporter() = delete;
	};
}
