#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Resource/Mesh/MeshResourceLayout.h>
#include <Runtime/Resource/Texture/TextureResourceLayout.h>
#include <Runtime/Resource/Material/MaterialResourceLayout.h>

namespace Hollow
{
	class RUNTIME_API ResourceImporter final
	{
	public:
		static MeshResourceLayout ImportMesh(String path);
		static TextureResourceLayout ImportTexture(String path);
		static MaterialResourceLayout ImportMaterial(String path);

	public:
		ResourceImporter() = delete;
		~ResourceImporter() = delete;
	};
}
