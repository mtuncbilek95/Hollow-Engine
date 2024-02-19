#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureType.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTextureUtils
	{
	public:
		static VkFormat GetVkFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::Unknown: return VK_FORMAT_UNDEFINED;
			case TextureFormat::RGBA32_Float: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::RGBA32_UInt: return VK_FORMAT_R32G32B32A32_UINT;
			case TextureFormat::RGBA32_Int: return VK_FORMAT_R32G32B32A32_SINT;

			case TextureFormat::RGB32_Float: return VK_FORMAT_R32G32B32_SFLOAT;
			case TextureFormat::RGB32_UInt: return VK_FORMAT_R32G32B32_UINT;
			case TextureFormat::RGB32_Int: return VK_FORMAT_R32G32B32_SINT;

			case TextureFormat::RGBA16_Float: return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA16_UNorm: return VK_FORMAT_R16G16B16A16_UNORM;
			case TextureFormat::RGBA16_UInt: return VK_FORMAT_R16G16B16A16_UINT;
			case TextureFormat::RGBA16_Norm: return VK_FORMAT_R16G16B16A16_SNORM;
			case TextureFormat::RGBA16_Int: return VK_FORMAT_R16G16B16A16_SINT;

			case TextureFormat::RG32_Float: return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RG32_UInt: return VK_FORMAT_R32G32_UINT;
			case TextureFormat::RG32_Int: return VK_FORMAT_R32G32_SINT;

			case TextureFormat::RGBA8_UNorm: return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8_UInt: return VK_FORMAT_R8G8B8A8_UINT;
			case TextureFormat::RGBA8_Norm: return VK_FORMAT_R8G8B8A8_SNORM;
			case TextureFormat::RGBA8_Int: return VK_FORMAT_R8G8B8A8_SINT;

			case TextureFormat::RG16_Float: return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG16_UNorm: return VK_FORMAT_R16G16_UNORM;
			case TextureFormat::RG16_UInt: return VK_FORMAT_R16G16_UINT;
			case TextureFormat::RG16_Norm: return VK_FORMAT_R16G16_SNORM;
			case TextureFormat::RG16_Int: return VK_FORMAT_R16G16_SINT;

			case TextureFormat::D32_Float: return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::R32_Float: return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::R32_UInt: return VK_FORMAT_R32_UINT;
			case TextureFormat::R32_Int: return VK_FORMAT_R32_SINT;

			case TextureFormat::D24_UNorm_S8_UInt: return VK_FORMAT_D24_UNORM_S8_UINT;
			case TextureFormat::D32_Float_S8_UInt: return VK_FORMAT_D32_SFLOAT_S8_UINT;

			case TextureFormat::RG8_UNorm: return VK_FORMAT_R8G8_UNORM;
			case TextureFormat::RG8_UInt: return VK_FORMAT_R8G8_UINT;
			case TextureFormat::RG8_Norm: return VK_FORMAT_R8G8_SNORM;
			case TextureFormat::RG8_Int: return VK_FORMAT_R8G8_SINT;

			case TextureFormat::R16_Float: return VK_FORMAT_R16_SFLOAT;
			case TextureFormat::D16_UNorm: return VK_FORMAT_D16_UNORM;
			case TextureFormat::R16_UNorm: return VK_FORMAT_R16_UNORM;
			case TextureFormat::R16_UInt: return VK_FORMAT_R16_UINT;
			case TextureFormat::R16_Norm: return VK_FORMAT_R16_SNORM;
			case TextureFormat::R16_Int: return VK_FORMAT_R16_SINT;

			case TextureFormat::R8_UNorm: return VK_FORMAT_R8_UNORM;
			case TextureFormat::R8_UInt: return VK_FORMAT_R8_UINT;
			case TextureFormat::R8_Norm: return VK_FORMAT_R8_SNORM;
			case TextureFormat::R8_Int: return VK_FORMAT_R8_SINT;

			default: return VK_FORMAT_UNDEFINED;
			}
		}

		static VkImageType GetVkImageType(TextureType type)
		{
			switch (type)
			{
			case TextureType::Texture1D: return VK_IMAGE_TYPE_1D;
			case TextureType::Texture2D: return VK_IMAGE_TYPE_2D;
			case TextureType::Texture3D: return VK_IMAGE_TYPE_3D;
			default: return VK_IMAGE_TYPE_2D;
			}
		}
	private:
		VulkanTextureUtils() = delete;
		~VulkanTextureUtils() = delete;
	};
}
