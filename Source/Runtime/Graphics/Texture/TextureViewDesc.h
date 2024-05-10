#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>

namespace Hollow
{
	class Texture;

	struct RUNTIME_API TextureViewDesc
	{
		Texture* pTexture;
		TextureAspectFlags AspectFlags;
		byte MipLevel;
		byte ArrayLayer;

	};
}