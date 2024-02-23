#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanShaderUtils
	{
	public:
		static VkShaderStageFlags GetVkShaderStage(ShaderStage stage)
		{
            VkShaderStageFlags flags = VkShaderStageFlags();

            if (stage & ShaderStage::Vertex)
                flags |= VK_SHADER_STAGE_VERTEX_BIT;
            if (stage & ShaderStage::Pixel)
                flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
            if (stage & ShaderStage::TesellationEval)
                flags |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            if (stage & ShaderStage::TesellationControl)
                flags |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            if (stage & ShaderStage::Geometry)
                flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
            if (stage & ShaderStage::Compute)
                flags |= VK_SHADER_STAGE_COMPUTE_BIT;

            return flags;
		}

    public:
        VulkanShaderUtils() = delete;
        ~VulkanShaderUtils() = delete;
	};
}