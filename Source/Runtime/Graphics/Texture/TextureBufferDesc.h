#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>

namespace Hollow
{
	struct RUNTIME_API TextureBufferDesc
	{
		SharedPtr<Texture> pTexture;
		TextureAspectFlags AspectFlags;
		byte MipLevel;
		byte ArrayLayer;
	};
}
