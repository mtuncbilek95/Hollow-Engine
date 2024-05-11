#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>

namespace Hollow
{
	class RUNTIME_API TextureUtils
	{
	public:
		static uint32 GetTextureFormatSize(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::Unknown:			return 0;
			case TextureFormat::RGBA32_Float:		return 16;
			case TextureFormat::RGBA32_UInt:		return 16;
			case TextureFormat::RGBA32_Int:			return 16;

			case TextureFormat::RGB32_Float:		return 12;
			case TextureFormat::RGB32_UInt:			return 12;
			case TextureFormat::RGB32_Int:			return 12;

			case TextureFormat::RGBA16_Float:		return 8;
			case TextureFormat::RGBA16_UNorm:		return 8;
			case TextureFormat::RGBA16_UInt:		return 8;
			case TextureFormat::RGBA16_Norm:		return 8;
			case TextureFormat::RGBA16_Int:			return 8;

			case TextureFormat::RG32_Float:			return 8;
			case TextureFormat::RG32_UInt:			return 8;
			case TextureFormat::RG32_Int:			return 8;

			case TextureFormat::RGBA8_UNorm:		return 4;
			case TextureFormat::RGBA8_UInt:			return 4;
			case TextureFormat::RGBA8_Norm:			return 4;
			case TextureFormat::RGBA8_Int:			return 4;

			case TextureFormat::RG16_Float:			return 4;
			case TextureFormat::RG16_UNorm:			return 4;
			case TextureFormat::RG16_UInt:			return 4;
			case TextureFormat::RG16_Norm:			return 4;
			case TextureFormat::RG16_Int:			return 4;

			case TextureFormat::D32_Float:			return 4;
			case TextureFormat::R32_Float:			return 4;
			case TextureFormat::R32_UInt:			return 4;
			case TextureFormat::R32_Int:			return 4;

			case TextureFormat::D24_UNorm_S8_UInt:	return 4;
			case TextureFormat::D32_Float_S8_UInt:	return 5;

			case TextureFormat::RG8_UNorm:			return 2;
			case TextureFormat::RG8_UInt:			return 2;
			case TextureFormat::RG8_Norm:			return 2;
			case TextureFormat::RG8_Int:			return 2;

			case TextureFormat::R16_Float:			return 2;
			case TextureFormat::D16_UNorm:			return 2;
			case TextureFormat::R16_UNorm:			return 2;
			case TextureFormat::R16_UInt:			return 2;
			case TextureFormat::R16_Norm:			return 2;
			case TextureFormat::R16_Int:			return 2;

			case TextureFormat::R8_UNorm:			return 1;
			case TextureFormat::R8_UInt:			return 1;
			case TextureFormat::R8_Norm:			return 1;
			case TextureFormat::R8_Int:				return 1;

			default:								return 0;
			}
		}

	public:
		TextureUtils() = delete;
		~TextureUtils() = delete;
	};
}