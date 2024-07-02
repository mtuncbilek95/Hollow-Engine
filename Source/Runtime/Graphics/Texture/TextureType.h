#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API TextureType : byte
	{
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,

		Texture1DMS,
		Texture2DMS,
		Texture3DMS,
		TextureCubeMS
	};
}