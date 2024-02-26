#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Shader/ShaderType.h>
#include <Runtime/Graphics/Shader/ShaderStage.h>

#include <Runtime/Graphics/Pipeline/RasterizerState/FaceCullMode.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/PolygonMode.h>
#include <Runtime/Graphics/Pipeline/RasterizerState/FaceDirection.h>

#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilOperation.h>
#include <Runtime/Graphics/Pipeline/DepthStencilState/StencilFaceDesc.h>

#include <Runtime/Graphics/Pipeline/BlendState/BlendColorWriteMask.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendFactor.h>

#include <Runtime/Graphics/Pipeline/InputLayout/InputBindingStepRate.h>
#include <Runtime/Graphics/Pipeline/InputLayout/MeshTopology.h>

#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

#include <Runtime/Vulkan/Common/VulkanCommonUtils.h>

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

		static VkPrimitiveTopology GetVkMeshTopology(MeshTopology topology)
		{
			switch (topology)
			{
			case MeshTopology::TriangleList:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case MeshTopology::TriangleStrip:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case MeshTopology::LineList:
				return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case MeshTopology::LineStrip:
				return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case MeshTopology::PointList:
				return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			default:
				return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			}
		}

		static VkPolygonMode GetVkPolygonMode(PolygonMode mode)
		{
			switch (mode)
			{
			case PolygonMode::Fill:
				return VK_POLYGON_MODE_FILL;
			case PolygonMode::Line:
				return VK_POLYGON_MODE_LINE;
			case PolygonMode::Point:
				return VK_POLYGON_MODE_POINT;
			default:
				return VK_POLYGON_MODE_FILL;
			}
		}

		static VkCullModeFlagBits GetVkFaceCullMode(FaceCullMode mode)
		{
			switch (mode)
			{
			case FaceCullMode::None:
				return VK_CULL_MODE_NONE;
			case FaceCullMode::Front:
				return VK_CULL_MODE_FRONT_BIT;
			case FaceCullMode::Back:
				return VK_CULL_MODE_BACK_BIT;
			default:
				return VK_CULL_MODE_BACK_BIT;
			}
		}

		static VkFrontFace GetVkFaceDirection(FaceDirection ordering)
		{
			switch (ordering)
			{
			case FaceDirection::Clockwise:
				return VK_FRONT_FACE_CLOCKWISE;
			case FaceDirection::CounterClockwise:
				return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			default:
				return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			}
		}

		static VkStencilOp GetVkStencilOperation(StencilOperation operation)
		{
			switch (operation)
			{
			case StencilOperation::Keep:
				return VK_STENCIL_OP_KEEP;
			case StencilOperation::Zero:
				return VK_STENCIL_OP_ZERO;
			case StencilOperation::Replace:
				return VK_STENCIL_OP_REPLACE;
			case StencilOperation::IncrementAndClamp:
				return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			case StencilOperation::DecrementAndClamp:
				return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			case StencilOperation::Invert:
				return VK_STENCIL_OP_INVERT;
			case StencilOperation::IncrementAndWrap:
				return VK_STENCIL_OP_INCREMENT_AND_WRAP;
			case StencilOperation::DecrementAndWrap:
				return VK_STENCIL_OP_DECREMENT_AND_WRAP;
			default:
				return VK_STENCIL_OP_MAX_ENUM;
			}
		}

		static VkColorComponentFlags GetVkBlendColorWriteMask(BlendColorWriteMask mask)
		{
			switch (mask)
			{
			case BlendColorWriteMask::R:
				return VK_COLOR_COMPONENT_R_BIT;
			case BlendColorWriteMask::G:
				return VK_COLOR_COMPONENT_G_BIT;
			case BlendColorWriteMask::B:
				return VK_COLOR_COMPONENT_B_BIT;
			case BlendColorWriteMask::A:
				return VK_COLOR_COMPONENT_A_BIT;
			case BlendColorWriteMask::All:
				return VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			default:
				return VK_COLOR_COMPONENT_FLAG_BITS_MAX_ENUM;
			}
		}

		static VkBlendOp GetVkBlendOperation(BlendOperation operation) noexcept
		{
			switch (operation)
			{
			case BlendOperation::Add:
				return VK_BLEND_OP_ADD;
			case BlendOperation::Subtract:
				return VK_BLEND_OP_SUBTRACT;
			case BlendOperation::ReverseSubtract:
				return VK_BLEND_OP_REVERSE_SUBTRACT;
			case BlendOperation::Min:
				return VK_BLEND_OP_MIN;
			case BlendOperation::Max:
				return VK_BLEND_OP_MAX;
			default:
				return VK_BLEND_OP_MAX_ENUM;
			}
		}

		static VkVertexInputRate GetVkInputBindingStepRate(InputBindingStepRate stepRate) noexcept
		{
			switch (stepRate)
			{
			case InputBindingStepRate::Vertex:
				return VK_VERTEX_INPUT_RATE_VERTEX;
			case InputBindingStepRate::Instance:
				return VK_VERTEX_INPUT_RATE_INSTANCE;
			default:
				return VK_VERTEX_INPUT_RATE_MAX_ENUM;
			}
		}

		static VkBlendFactor GetVkBlendFactor(const BlendFactor factor) noexcept
		{
			switch (factor)
			{
			case BlendFactor::Zero:
				return VK_BLEND_FACTOR_ZERO;
			case BlendFactor::One:
				return VK_BLEND_FACTOR_ONE;
			case BlendFactor::SrcColor:
				return VK_BLEND_FACTOR_SRC_COLOR;
			case BlendFactor::OneMinusSrcColor:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case BlendFactor::DstColor:
				return VK_BLEND_FACTOR_DST_COLOR;
			case BlendFactor::OneMinusDstColor:
				return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case BlendFactor::SrcAlpha:
				return VK_BLEND_FACTOR_SRC_ALPHA;
			case BlendFactor::OneMinusSrcAlpha:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case BlendFactor::DstAlpha:
				return VK_BLEND_FACTOR_DST_ALPHA;
			case BlendFactor::OneMinusDstAlpha:
				return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case BlendFactor::ConstantColor:
				return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case BlendFactor::OneMinusConstantColor:
				return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case BlendFactor::ConstantAlpha:
				return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case BlendFactor::OneMinusConstantAlpha:
				return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			case BlendFactor::SrcAlphaSat:
				return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			case BlendFactor::Src1Color:
				return VK_BLEND_FACTOR_SRC1_COLOR;
			case BlendFactor::OneMinusSrc1Color:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
			case BlendFactor::Src1Alpha:
				return VK_BLEND_FACTOR_SRC1_ALPHA;
			case BlendFactor::OneMinusSrc1Alpha:
				return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
			default:
				return VK_BLEND_FACTOR_MAX_ENUM;
			}
		}

		static VkStencilOpState GetVkStencilFaceDesc(StencilFaceDesc state)
		{
			VkStencilOpState vkState;
			vkState.failOp = GetVkStencilOperation(state.FailOperation);
			vkState.passOp = GetVkStencilOperation(state.PassOperation);
			vkState.depthFailOp = GetVkStencilOperation(state.DepthFailOperation);
			vkState.compareOp = VulkanCommonUtils::GetVkCompareOperation(state.CompareOperation);
			vkState.compareMask = state.CompareMask;
			vkState.writeMask = state.WriteMask;
			vkState.reference = state.Reference;
			return vkState;
		}

		static VkPipelineStageFlags GetVkPipelineStageFlags(const PipelineStageFlags stages)
		{
			VkPipelineStageFlags flags = 0;
			if (stages & PipelineStageFlags::BottomOfPipe)
				flags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			if (stages & PipelineStageFlags::ColorAttachmentOutput)
				flags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			if (stages & PipelineStageFlags::ComputeShader)
				flags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			if (stages & PipelineStageFlags::EarlyFragmentTests)
				flags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			if (stages & PipelineStageFlags::FragmentShader)
				flags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			if (stages & PipelineStageFlags::GeometryShader)
				flags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
			if (stages & PipelineStageFlags::LateFragmentTests)
				flags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			if (stages & PipelineStageFlags::TessellationControlShader)
				flags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
			if (stages & PipelineStageFlags::TessellationEvaluationShader)
				flags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
			if (stages & PipelineStageFlags::TopOfPipe)
				flags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			if (stages & PipelineStageFlags::Transfer)
				flags |= VK_PIPELINE_STAGE_TRANSFER_BIT;
			if (stages & PipelineStageFlags::VertexInput)
				flags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			if (stages & PipelineStageFlags::VertexShader)
				flags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;

			return flags;
		}

		static VkPipelineBindPoint GetVkPipelineBindPoint(PipelineBindPoint bindPoint)
		{
			switch (bindPoint)
			{
			case PipelineBindPoint::Graphics:
				return VK_PIPELINE_BIND_POINT_GRAPHICS;
			case PipelineBindPoint::Compute:
				return VK_PIPELINE_BIND_POINT_COMPUTE;
			default:
				return VK_PIPELINE_BIND_POINT_GRAPHICS;
			}
		}
	public:
		VulkanPipelineUtils() = delete;
		~VulkanPipelineUtils() = delete;
	};
}