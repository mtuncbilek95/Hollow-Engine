#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Resource/Texture/TextureResourceLayout.h>

namespace Hollow
{
	struct RUNTIME_API SubMeshMaterial
	{
		u32 MaterialIndex;
		SharedPtr<TextureResourceLayout> BaseTexture;
	};

	struct RUNTIME_API MaterialResourceLayout
	{
		ArrayList<SubMeshMaterial> SubMeshMaterials;
	};
}
