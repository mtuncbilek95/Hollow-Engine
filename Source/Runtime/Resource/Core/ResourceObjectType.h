#pragma once

#include <Runtime/Core/Core.h>

namespace Hollow
{
	enum class RUNTIME_API ResourceObjectType
	{
		Unknown,
		Mesh,
		Texture,
		Material,
		RenderTarget,
		Shader,
		Skybox,
		Scene,
		Animation,
		Audio,
		Font,
		Script,
		Count
	};
}
