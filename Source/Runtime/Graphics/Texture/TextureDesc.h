#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>

namespace Hollow
{
	class GraphicsMemory;
	struct RUNTIME_API TextureDesc
	{
		Vector3u ImageSize;
		byte ArraySize;
		byte MipLevels;
		TextureFormat Format;
		TextureUsage Usages;
		TextureSampleCount SampleCount;
		TextureType Type;
		
		SharedPtr<GraphicsMemory> pMemory;
	};
}
