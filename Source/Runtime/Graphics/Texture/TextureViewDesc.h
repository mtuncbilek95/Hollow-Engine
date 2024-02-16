#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>

namespace Hollow
{
	struct RUNTIME_API TextureViewDesc
	{
		byte ArraySize;
		byte MipLevels;
		SharedPtr<Texture> pTexture;
	};
}
