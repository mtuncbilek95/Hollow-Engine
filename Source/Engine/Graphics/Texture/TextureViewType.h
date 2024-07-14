#pragma once

#include <Engine/Core/Core.h>

namespace Hollow
{
	enum class ENGINE_API TextureViewType : u8
	{
		Undefined,
		Texture1D,
		Texture2D,
		Texture3D,
		TextureCube,
		Texture1DArray,
		Texture2DArray,
		TextureCubeArray
	};
}