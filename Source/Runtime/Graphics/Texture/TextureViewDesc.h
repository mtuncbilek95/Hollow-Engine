#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureAspect.h>

namespace Hollow
{
	struct RUNTIME_API TextureViewDesc
	{
		byte ArraySize;
		byte MipLevels;
		Array<TextureAspect> Aspects;
		SharedPtr<Texture> pTexture;
	};
}
