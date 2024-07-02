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
		TextureType Type;
		byte MipLevel;
		byte MipCount;
		byte ArrayLayer;
		byte ArrayCount;
	};
}
