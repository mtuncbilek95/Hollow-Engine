#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>

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

		Vec2u ImageSize;
	};
}
