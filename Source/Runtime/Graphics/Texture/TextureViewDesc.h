#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>

namespace Hollow
{
	struct RUNTIME_API TextureViewDesc
	{
		byte ArraySize;
		byte MipLevels;
		Array<TextureAspectFlags> Aspects;
		SharedPtr<Texture> pTexture;
	};
}
