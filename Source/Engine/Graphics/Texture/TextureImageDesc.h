#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureFormat.h>
#include <Engine/Graphics/Texture/TextureUsageFlags.h>
#include <Engine/Graphics/Texture/TextureType.h>
#include <Engine/Graphics/Texture/SampleCount.h>
#include <Engine/Graphics/Texture/TextureCreateFlags.h>

namespace Hollow
{
	class GraphicsMemory;
	struct TextureImageDesc
	{
		Vec3u ImageSize;
		u32 MipLevels;
		u32 ArrayLayers;
		TextureFormat ImageFormat;
		TextureUsageFlags UsageFlags;
		TextureType ImageType;
		SampleCount Samples;
		TextureCreateFlags CreateFlags;

		SharedPtr<GraphicsMemory> pRequestMemory;
	};
}
