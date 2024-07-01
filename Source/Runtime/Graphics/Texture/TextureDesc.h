#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureCreateFlags.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>

namespace Hollow
{
	struct RUNTIME_API TextureDesc
	{
		Vec3u ImageSize;
		byte ArraySize;
		byte MipLevels;
		TextureFormat ImageFormat;
		TextureUsage Usage;
		TextureSampleCount SampleCount;
		TextureType Type;
		TextureCreateFlags Flags;

		SharedPtr<GraphicsMemory> pMemory;
	};
}
