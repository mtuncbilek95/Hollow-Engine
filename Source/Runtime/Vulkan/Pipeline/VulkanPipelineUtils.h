#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Runtime/Graphics/Pipeline/InputLayout/MeshTopology.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/FaceCullMode.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendFactor.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendColorWriteMask.h>
#include <Runtime/Graphics/Pipeline/PipelineBindPoint.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanPipelineUtils
	{
	public:
		static VkPrimitiveTopology GetVkPrimitiveTopology(MeshTopology topology)
		{
			switch (topology)
			{
			case MeshTopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case MeshTopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case MeshTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case MeshTopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case MeshTopology::PatchList: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			case MeshTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case MeshTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			default: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			}
		}

		static VkPolygonMode GetVkPolygonMode(PolygonMode mode)
		{
			switch (mode)
			{
			case PolygonMode::Fill: return VK_POLYGON_MODE_FILL;
			case PolygonMode::Line: return VK_POLYGON_MODE_LINE;
			case PolygonMode::Point: return VK_POLYGON_MODE_POINT;
			default: return VK_POLYGON_MODE_FILL;
			}
		}

		static VkCullModeFlags GetVkCullMode(FaceCullMode mode)
		{
			VkCullModeFlags flags = 0;

			if (mode == FaceCullMode::None)
				return VK_CULL_MODE_NONE;

			if (mode & FaceCullMode::Front)
				flags |= VK_CULL_MODE_FRONT_BIT;
			if (mode & FaceCullMode::Back)
				flags |= VK_CULL_MODE_BACK_BIT;
			return flags;
		}

		static VkBlendFactor GetVkBlendFactor(BlendFactor factor)
		{
			switch (factor)
			{
			case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
			case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
			case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
			case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case BlendFactor::SrcAlphaSat: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case BlendFactor::Src1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
			case BlendFactor::OneMinusSrc1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case BlendFactor::Src1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
			case BlendFactor::OneMinusSrc1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			default: return VK_BLEND_FACTOR_ONE;
			}
		}

		static VkBlendOp GetVkBlendOperation(BlendOperation operation)
		{
			switch (operation)
			{
			case BlendOperation::Add: return VK_BLEND_OP_ADD;
			case BlendOperation::Subtract: return VK_BLEND_OP_SUBTRACT;
			case BlendOperation::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperation::Min: return VK_BLEND_OP_MIN;
			case BlendOperation::Max: return VK_BLEND_OP_MAX;
			default: return VK_BLEND_OP_ADD;
			}
		}

		static VkColorComponentFlags GetVkColorWriteMask(BlendColorWriteMask mask)
		{
			VkColorComponentFlags flags = 0;

			if (mask & BlendColorWriteMask::Red)
				flags |= VK_COLOR_COMPONENT_R_BIT;
			if (mask & BlendColorWriteMask::Green)
				flags |= VK_COLOR_COMPONENT_G_BIT;
			if (mask & BlendColorWriteMask::Blue)
				flags |= VK_COLOR_COMPONENT_B_BIT;
			if (mask & BlendColorWriteMask::Alpha)
				flags |= VK_COLOR_COMPONENT_A_BIT;

			return flags;
		}

		static VkVertexInputRate GetVkVertexInputRate(InputBindingStepRate rate)
		{
			switch (rate)
			{
			case InputBindingStepRate::Vertex: return VK_VERTEX_INPUT_RATE_VERTEX;
			case InputBindingStepRate::Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
			default: return VK_VERTEX_INPUT_RATE_VERTEX;
			}
		}

		static VkPipelineBindPoint GetVkPipelineBindPoint(PipelineBindPoint point)
		{
			switch (point)
			{
			case PipelineBindPoint::Graphics: return VK_PIPELINE_BIND_POINT_GRAPHICS;
			case PipelineBindPoint::Compute: return VK_PIPELINE_BIND_POINT_COMPUTE;
			default: return VK_PIPELINE_BIND_POINT_GRAPHICS;
			}
		}

	public:
		VulkanPipelineUtils() = delete;
		~VulkanPipelineUtils() = delete;
	};
}