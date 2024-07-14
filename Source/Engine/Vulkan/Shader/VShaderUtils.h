#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Shader/ShaderStage.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	namespace Utils
	{
		static VkShaderStageFlags GetVkShaderStage(ShaderStage stage)
		{
			VkShaderStageFlags vkStage = 0;

			if (stage & ShaderStage::Vertex)
				vkStage |= VK_SHADER_STAGE_VERTEX_BIT;
			if (stage & ShaderStage::TessellationControl)
				vkStage |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			if (stage & ShaderStage::TessellationEvaluation)
				vkStage |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			if (stage & ShaderStage::Geometry)
				vkStage |= VK_SHADER_STAGE_GEOMETRY_BIT;
			if (stage & ShaderStage::Fragment)
				vkStage |= VK_SHADER_STAGE_FRAGMENT_BIT;
			if (stage & ShaderStage::Compute)
				vkStage |= VK_SHADER_STAGE_COMPUTE_BIT;

			return vkStage;
		}

		static VkShaderStageFlagBits GetVkShaderStageBit(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::TessellationControl:
				return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case ShaderStage::TessellationEvaluation:
				return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case ShaderStage::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case ShaderStage::Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			default:
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
		}
	}
}