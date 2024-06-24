#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Resource/Texture/TextureResourceLayout.h>

namespace Hollow
{
	struct RUNTIME_API SubMeshMaterial
	{
		u32 MaterialIndex;
		TextureResourceLayout BaseTexture;
		TextureResourceLayout NormalTexture;
	};

	struct RUNTIME_API MaterialResourceLayout
	{
		DArray<SubMeshMaterial> SubMeshMaterials;
	};
}
