#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Shader/ShaderType.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>
#include <Runtime/Graphics/Pipeline/InputLayout/PrimitiveMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/FillMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/CullMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/FaceOrdering.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanPipelineUtils
	{
	public:
		static VkShaderStageFlagBits GetVkShaderStage(ShaderType type)
		{
			switch (type)
			{
			case ShaderType::Vertex:
				return VK_SHADER_STAGE_VERTEX_BIT;
			case ShaderType::Pixel:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderType::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case ShaderType::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderType::TessellationControl:
				return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case ShaderType::TessellationEvaluation:
				return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			default:
				return VK_SHADER_STAGE_VERTEX_BIT;
			}
		}

		static VkPrimitiveTopology GetVkPrimitiveMode(PrimitiveMode topology)
		{
			switch (topology)
			{
			case PrimitiveMode::TriangleList:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveMode::TriangleStrip:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case PrimitiveMode::LineList:
				return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveMode::LineStrip:
				return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case PrimitiveMode::PointList:
				return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			default:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			}
		}

		static VkPolygonMode GetVkPolygonMode(FillMode mode)
		{
			switch (mode)
			{
			case FillMode::Solid:
				return VK_POLYGON_MODE_FILL;
			case FillMode::Wireframe:
				return VK_POLYGON_MODE_LINE;
			default:
				return VK_POLYGON_MODE_FILL;
			}
		}

		static VkCullModeFlagBits GetVkCullMode(CullMode mode)
		{
			switch (mode)
			{
			case CullMode::None:
				return VK_CULL_MODE_NONE;
			case CullMode::Front:
				return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back:
				return VK_CULL_MODE_BACK_BIT;
			default:
				return VK_CULL_MODE_BACK_BIT;
			}
		}

		static VkFrontFace GetVkFrontFace(FaceOrdering ordering)
		{
			switch (ordering)
			{
			case FaceOrdering::Clockwise:
				return VK_FRONT_FACE_CLOCKWISE;
			case FaceOrdering::CounterClockwise:
				return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			default:
				return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			}
		}

	public:
		VulkanPipelineUtils() = delete;
		~VulkanPipelineUtils() = delete;
	};
}