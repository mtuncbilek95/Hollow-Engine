#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/SamplerAddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerBorderColor.h>
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>
#include <Runtime/Graphics/Sampler/SamplerMapMode.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanSamplerUtils
	{
	public:
		static VkFilter GetVkFiltering(SamplerFiltering filtering)
		{
			switch (filtering)
			{
			case SamplerFiltering::None:
				return VK_FILTER_LINEAR;
			case SamplerFiltering::Nearest:
				return VK_FILTER_NEAREST;
			case SamplerFiltering::Linear:
				return VK_FILTER_LINEAR;
			default:
				return VK_FILTER_NEAREST;
			}
		}
		static VkSamplerMipmapMode GetVkMapMode(SamplerMapMode mapMode)
		{
			switch (mapMode)
			{
			case SamplerMapMode::None:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;
			case SamplerMapMode::Nearest:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;
			case SamplerMapMode::Linear:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;
			default:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;
			}
		}
		static VkSamplerAddressMode GetVkAddressMode(SamplerAddressMode addressMode)
		{
			switch (addressMode)
			{
			case SamplerAddressMode::Repeat:
				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			case SamplerAddressMode::MirroredRepeat:
				return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
			case SamplerAddressMode::ClampToEdge:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			case SamplerAddressMode::ClampToBorder:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
			default:
				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			}
		
		}
		static VkBorderColor GetVkBorderColor(SamplerBorderColor borderColor)
		{
			switch (borderColor)
			{
			case SamplerBorderColor::TransparentBlack:
				return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			case SamplerBorderColor::OpaqueBlack:
				return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			case SamplerBorderColor::OpaqueWhite:
				return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			default:
				return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
			}
		}

	public:
		VulkanSamplerUtils() = delete;
		~VulkanSamplerUtils() = delete;
	};
}