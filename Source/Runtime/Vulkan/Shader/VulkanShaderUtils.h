#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanShaderUtils
	{
	public:
		static VkShaderStageFlags GetVkShaderStage(ShaderStage stages)
		{
			VkShaderStageFlags flags = VkShaderStageFlags();

			if (stages & ShaderStage::Vertex)
				flags |= VK_SHADER_STAGE_VERTEX_BIT;
			if (stages & ShaderStage::Fragment)
				flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
			if (stages & ShaderStage::TesellationEval)
				flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			if (stages & ShaderStage::TesellationControl)
				flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			if (stages & ShaderStage::Geometry)
				flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
			if (stages & ShaderStage::Compute)
				flags |= VK_SHADER_STAGE_COMPUTE_BIT;

			return flags;
		}

		static VkShaderStageFlagBits GetVkShaderStageBit(ShaderStage stage)
		{
			switch (stage)
			{
			case ShaderStage::Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderStage::Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderStage::TesellationEval:
				return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case ShaderStage::TesellationControl:
				return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case ShaderStage::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case ShaderStage::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			default:
				return VK_SHADER_STAGE_ALL;
			}
		}

	public:
		VulkanShaderUtils() = delete;
		~VulkanShaderUtils() = delete;
	};
}