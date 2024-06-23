#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Resource/Texture/TextureResourceLayout.h>

namespace Hollow
{
	struct RUNTIME_API SubMeshMaterial
	{
		u32 MaterialIndex;
		TextureResourceLayout BaseTexture;
	};

	struct RUNTIME_API MaterialResourceLayout
	{
		DArray<SubMeshMaterial> SubMeshMaterials;
	};
}
