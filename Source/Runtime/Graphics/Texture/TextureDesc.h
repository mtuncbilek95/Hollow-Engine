#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Buffer/BufferCPUAccess.h>

namespace Hollow
{
	class GraphicsMemory;
	struct RUNTIME_API TextureDesc
	{
		Vector2u ImageSize;
		byte ArraySize;
		byte MipLevels;
		TextureFormat Format;
		TextureUsage Usage;
		BufferCPUAccess CPUAccess;
		TextureType Type;
		
		SharedPtr<GraphicsMemory> pMemory;
	};
}
