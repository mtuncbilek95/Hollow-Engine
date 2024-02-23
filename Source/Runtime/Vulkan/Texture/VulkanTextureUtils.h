#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureFormat.h>
#include <Runtime/Graphics/Texture/TextureType.h>
#include <Runtime/Graphics/Texture/TextureUsage.h>
#include <Runtime/Graphics/Texture/TextureSampleCount.h>
#include <Runtime/Graphics/Texture/TextureAspectFlags.h>
#include <Runtime/Graphics/Texture/TextureLayout.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanTextureUtils
	{
	public:
		static VkFormat GetVkTextureFormat(TextureFormat format)
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

		static VkImageType GetVkTextureType(TextureType type)
		{
			switch (type)
			{
			case TextureType::Texture1D: return VK_IMAGE_TYPE_1D;
			case TextureType::Texture2D: return VK_IMAGE_TYPE_2D;
			case TextureType::Texture3D: return VK_IMAGE_TYPE_3D;
			default: return VK_IMAGE_TYPE_2D;
			}
		}

		static VkImageUsageFlags GetVkTextureUsageFlags(Array<TextureUsage> usages)
		{
			VkImageUsageFlags flags = 0;

			for (auto usage : usages)
			{
				switch (usage)
				{
				case TextureUsage::Sampled:						flags |= VK_IMAGE_USAGE_SAMPLED_BIT; break;
				case TextureUsage::Storage:						flags |= VK_IMAGE_USAGE_STORAGE_BIT; break;
				case TextureUsage::ColorAttachment:				flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; break;
				case TextureUsage::DepthStencilAttachment:		flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; break;
				case TextureUsage::TransferSource:				flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; break;
				case TextureUsage::TransferDestination:			flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT; break;
				case TextureUsage::TransientAttachment:			flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT; break;
				case TextureUsage::InputAttachment:				flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT; break;
				default:										flags |= VK_IMAGE_USAGE_SAMPLED_BIT; break;
				}
			}

			return flags;
		}

		static VkSampleCountFlagBits GetVkSampleCount(TextureSampleCount sample)
		{
			switch (sample)
			{
			case TextureSampleCount::Sample1:	return VK_SAMPLE_COUNT_1_BIT;
			case TextureSampleCount::Sample2:	return VK_SAMPLE_COUNT_2_BIT;
			case TextureSampleCount::Sample4:	return VK_SAMPLE_COUNT_4_BIT;
			case TextureSampleCount::Sample8:	return VK_SAMPLE_COUNT_8_BIT;
			case TextureSampleCount::Sample16:	return VK_SAMPLE_COUNT_16_BIT;
			case TextureSampleCount::Sample32:	return VK_SAMPLE_COUNT_32_BIT;
			case TextureSampleCount::Sample64:	return VK_SAMPLE_COUNT_64_BIT;
			default:							return VK_SAMPLE_COUNT_1_BIT;
			}
		}

		static VkImageViewType GetTextureViewType(TextureType type)
		{
			switch (type)
			{
			case TextureType::Texture1D:	return VK_IMAGE_VIEW_TYPE_1D;
			case TextureType::Texture2D:	return VK_IMAGE_VIEW_TYPE_2D;
			case TextureType::Texture3D:	return VK_IMAGE_VIEW_TYPE_3D;
			default:						return VK_IMAGE_VIEW_TYPE_2D;
			}
		}

		static VkImageAspectFlags GetVkTextureAspects(Array<TextureAspectFlags> aspects)
		{
			VkImageAspectFlags flags = 0;

			for (auto aspect : aspects)
			{
				switch (aspect)
				{
				case TextureAspectFlags::ColorAspect: flags |= VK_IMAGE_ASPECT_COLOR_BIT; break;
				case TextureAspectFlags::DepthAspect: flags |= VK_IMAGE_ASPECT_DEPTH_BIT; break;
				case TextureAspectFlags::StencilAspect: flags |= VK_IMAGE_ASPECT_STENCIL_BIT; break;
				default: break;
				}
			}

			return flags;
		}

		static VkImageLayout GetVkTextureLayout(TextureLayout usage)
		{
			switch (usage)
			{
			case TextureLayout::Unknown:					return VK_IMAGE_LAYOUT_UNDEFINED;
			case TextureLayout::General:					return VK_IMAGE_LAYOUT_GENERAL;
			case TextureLayout::ColorAttachment:			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case TextureLayout::DepthStencilAttachment:		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case TextureLayout::ShaderReadOnly:				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case TextureLayout::TransferSource:				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case TextureLayout::TransferDestination:		return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case TextureLayout::Present:					return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			default:										return VK_IMAGE_LAYOUT_UNDEFINED;
			}
		}

	private:
		VulkanTextureUtils() = delete;
		~VulkanTextureUtils() = delete;
	};
}
