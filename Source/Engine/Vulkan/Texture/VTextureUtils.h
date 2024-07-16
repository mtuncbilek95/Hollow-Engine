#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureType.h>
#include <Engine/Graphics/Texture/TextureFormat.h>
#include <Engine/Graphics/Texture/TextureUsageFlags.h>
#include <Engine/Graphics/Texture/SampleCount.h>
#include <Engine/Graphics/Texture/TextureCreateFlags.h>
#include <Engine/Graphics/Texture/TextureViewType.h>
#include <Engine/Graphics/Texture/TextureAspectFlags.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace VkUtils
	{
		static VkImageType GetVkImageType(TextureType type)
		{
			switch (type)
			{
			case TextureType::Texture1D:
				return VK_IMAGE_TYPE_1D;
			case TextureType::Texture2D:
				return VK_IMAGE_TYPE_2D;
			case TextureType::Texture3D:
				return VK_IMAGE_TYPE_3D;
			default:
				return VK_IMAGE_TYPE_MAX_ENUM;
			}
		}

		static VkFormat GetVkFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::Unknown:			return VK_FORMAT_UNDEFINED;
			case TextureFormat::RGBA32_Float:		return VK_FORMAT_R32G32B32A32_SFLOAT;
			case TextureFormat::RGBA32_UInt:		return VK_FORMAT_R32G32B32A32_UINT;
			case TextureFormat::RGBA32_Int:			return VK_FORMAT_R32G32B32A32_SINT;

			case TextureFormat::RGB32_Float:		return VK_FORMAT_R32G32B32_SFLOAT;
			case TextureFormat::RGB32_UInt:			return VK_FORMAT_R32G32B32_UINT;
			case TextureFormat::RGB32_Int:			return VK_FORMAT_R32G32B32_SINT;

			case TextureFormat::RGBA16_Float:		return VK_FORMAT_R16G16B16A16_SFLOAT;
			case TextureFormat::RGBA16_UNorm:		return VK_FORMAT_R16G16B16A16_UNORM;
			case TextureFormat::RGBA16_UInt:		return VK_FORMAT_R16G16B16A16_UINT;
			case TextureFormat::RGBA16_Norm:		return VK_FORMAT_R16G16B16A16_SNORM;
			case TextureFormat::RGBA16_Int:			return VK_FORMAT_R16G16B16A16_SINT;

			case TextureFormat::RG32_Float:			return VK_FORMAT_R32G32_SFLOAT;
			case TextureFormat::RG32_UInt:			return VK_FORMAT_R32G32_UINT;
			case TextureFormat::RG32_Int:			return VK_FORMAT_R32G32_SINT;

			case TextureFormat::RGBA8_UNorm:		return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8_UInt:			return VK_FORMAT_R8G8B8A8_UINT;
			case TextureFormat::RGBA8_Norm:			return VK_FORMAT_R8G8B8A8_SNORM;
			case TextureFormat::RGBA8_Int:			return VK_FORMAT_R8G8B8A8_SINT;
			case TextureFormat::RGBA8_SRGB:			return VK_FORMAT_R8G8B8A8_SRGB;

			case TextureFormat::RG16_Float:			return VK_FORMAT_R16G16_SFLOAT;
			case TextureFormat::RG16_UNorm:			return VK_FORMAT_R16G16_UNORM;
			case TextureFormat::RG16_UInt:			return VK_FORMAT_R16G16_UINT;
			case TextureFormat::RG16_Norm:			return VK_FORMAT_R16G16_SNORM;
			case TextureFormat::RG16_Int:			return VK_FORMAT_R16G16_SINT;

			case TextureFormat::D32_Float:			return VK_FORMAT_D32_SFLOAT;
			case TextureFormat::R32_Float:			return VK_FORMAT_R32_SFLOAT;
			case TextureFormat::R32_UInt:			return VK_FORMAT_R32_UINT;
			case TextureFormat::R32_Int:			return VK_FORMAT_R32_SINT;

			case TextureFormat::D24_UNorm_S8_UInt:	return VK_FORMAT_D24_UNORM_S8_UINT;
			case TextureFormat::D32_Float_S8_UInt:	return VK_FORMAT_D32_SFLOAT_S8_UINT;

			case TextureFormat::RG8_UNorm:			return VK_FORMAT_R8G8_UNORM;
			case TextureFormat::RG8_UInt:			return VK_FORMAT_R8G8_UINT;
			case TextureFormat::RG8_Norm:			return VK_FORMAT_R8G8_SNORM;
			case TextureFormat::RG8_Int:			return VK_FORMAT_R8G8_SINT;

			case TextureFormat::R16_Float:			return VK_FORMAT_R16_SFLOAT;
			case TextureFormat::D16_UNorm:			return VK_FORMAT_D16_UNORM;
			case TextureFormat::R16_UNorm:			return VK_FORMAT_R16_UNORM;
			case TextureFormat::R16_UInt:			return VK_FORMAT_R16_UINT;
			case TextureFormat::R16_Norm:			return VK_FORMAT_R16_SNORM;
			case TextureFormat::R16_Int:			return VK_FORMAT_R16_SINT;

			case TextureFormat::R8_UNorm:			return VK_FORMAT_R8_UNORM;
			case TextureFormat::R8_UInt:			return VK_FORMAT_R8_UINT;
			case TextureFormat::R8_Norm:			return VK_FORMAT_R8_SNORM;
			case TextureFormat::R8_Int:				return VK_FORMAT_R8_SINT;

			default:								return VK_FORMAT_UNDEFINED;
			}
		}

		static VkImageUsageFlags GetVkImageUsageFlags(TextureUsageFlags flags)
		{
			VkImageUsageFlags vkFlags = 0;

			if (flags & TextureUsageFlags::SampledImage)
				vkFlags |= VK_IMAGE_USAGE_SAMPLED_BIT;

			if (flags & TextureUsageFlags::ColorAttachment)
				vkFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			else if (flags & TextureUsageFlags::DepthStencilAttachment)
				vkFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

			if (flags & TextureUsageFlags::StorageImage)
				vkFlags |= VK_IMAGE_USAGE_STORAGE_BIT;
			if (flags & TextureUsageFlags::TransferDestination)
				vkFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			else if (flags & TextureUsageFlags::TransferSource)
				vkFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			return vkFlags;
		}

		static VkSampleCountFlagBits GetVkSampleCountFlags(SampleCount count)
		{
			switch (count)
			{
			case SampleCount::Sample1:	return VK_SAMPLE_COUNT_1_BIT;
			case SampleCount::Sample2:	return VK_SAMPLE_COUNT_2_BIT;
			case SampleCount::Sample4:	return VK_SAMPLE_COUNT_4_BIT;
			case SampleCount::Sample8:	return VK_SAMPLE_COUNT_8_BIT;
			case SampleCount::Sample16:	return VK_SAMPLE_COUNT_16_BIT;
			case SampleCount::Sample32:	return VK_SAMPLE_COUNT_32_BIT;
			case SampleCount::Sample64:	return VK_SAMPLE_COUNT_64_BIT;
			default:					return VK_SAMPLE_COUNT_1_BIT;
			}
		}

		static VkImageCreateFlags GetVkImageCreateFlags(TextureCreateFlags flags)
		{
			VkImageCreateFlags vkFlags = VkImageCreateFlags();

			if (flags & TextureCreateFlags::Undefined)
				return 0;

			if (flags & TextureCreateFlags::SparseBinding)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
			if (flags & TextureCreateFlags::SparseResidency)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
			if (flags & TextureCreateFlags::SparseAliased)
				vkFlags |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
			if (flags & TextureCreateFlags::MutableFormat)
				vkFlags |= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
			if (flags & TextureCreateFlags::CubeCompatible)
				vkFlags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::Array2D)
				vkFlags |= VK_IMAGE_CREATE_2D_ARRAY_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::SplitInstanceBindRegions)
				vkFlags |= VK_IMAGE_CREATE_SPLIT_INSTANCE_BIND_REGIONS_BIT;
			if (flags & TextureCreateFlags::BlockTexelView)
				vkFlags |= VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT;
			if (flags & TextureCreateFlags::ExtendedUsage)
				vkFlags |= VK_IMAGE_CREATE_EXTENDED_USAGE_BIT;
			if (flags & TextureCreateFlags::Disjoint)
				vkFlags |= VK_IMAGE_CREATE_DISJOINT_BIT;
			if (flags & TextureCreateFlags::Alias)
				vkFlags |= VK_IMAGE_CREATE_ALIAS_BIT;
			if (flags & TextureCreateFlags::Protected)
				vkFlags |= VK_IMAGE_CREATE_PROTECTED_BIT;

			return vkFlags;
		}

		static VkImageViewType GetVkTextureViewType(TextureViewType type)
		{
			switch (type)
			{
			case TextureViewType::Texture1D:		return VK_IMAGE_VIEW_TYPE_1D;
			case TextureViewType::Texture2D:		return VK_IMAGE_VIEW_TYPE_2D;
			case TextureViewType::Texture3D:		return VK_IMAGE_VIEW_TYPE_3D;
			case TextureViewType::TextureCube:		return VK_IMAGE_VIEW_TYPE_CUBE;
			case TextureViewType::Texture1DArray:	return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case TextureViewType::Texture2DArray:	return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			case TextureViewType::TextureCubeArray:	return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			default:								return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
			}
		}

		static VkImageAspectFlags GetVkTextureAspectFlags(TextureAspectFlags flags)
		{
			VkImageAspectFlags vkFlags = 0;

			if (flags & TextureAspectFlags::Undefined)
				return 0;

			if (flags & TextureAspectFlags::ColorAspect)
				vkFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
			if (flags & TextureAspectFlags::DepthAspect)
				vkFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
			if (flags & TextureAspectFlags::StencilAspect)
				vkFlags |= VK_IMAGE_ASPECT_STENCIL_BIT;
			if (flags & TextureAspectFlags::MetaAspect)
				vkFlags |= VK_IMAGE_ASPECT_METADATA_BIT;

			return vkFlags;
		}
	}
}
