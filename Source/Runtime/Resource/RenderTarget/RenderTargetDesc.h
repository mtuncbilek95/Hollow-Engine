#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>

namespace Hollow
{
	struct RUNTIME_API RenderTargetDesc
	{
		TextureFormat ColorFormat;
		TextureFormat DepthFormat;
		TextureType TargetType;
		TextureSampleCount SampleCount;
		TextureUsage ColorUsage;
		TextureUsage DepthUsage;
		u32 ColorAttachmentCount;
		Vec2u ImageSize;

		DArray<SharedPtr<Texture>> pColorTextures;
		DArray<SharedPtr<TextureBuffer>> pColorTextureBuffers;
		bool HasDepthTexture;
	};
}
