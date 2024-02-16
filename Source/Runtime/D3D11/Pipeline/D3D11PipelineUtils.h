#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Pipeline/BlendState/BlendOperation.h>
#include <Runtime/Graphics/Pipeline/BlendState/BlendType.h>
#include <Runtime/Graphics/Pipeline/BlendState/ColorMask.h>
#include <Runtime/Graphics/Pipeline/DepthStencil/DepthMask.h>
#include <Runtime/Graphics/Pipeline/DepthStencil/ComparisonFunc.h>
#include <Runtime/Graphics/Pipeline/InputLayout/SlotClass.h>
#include <Runtime/Graphics/Pipeline/InputLayout/PrimitiveMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/FillMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/CullMode.h>
#include <Runtime/Graphics/Pipeline/Rasterizer/FaceOrdering.h>

#include <d3d11.h>

namespace Hollow
{
	class RUNTIME_API D3D11PipelineUtils
	{
	public:
		static D3D11_BLEND_OP GetBlendOp(BlendOperation operation)
		{
			switch (operation)
			{
			case BlendOperation::Add:					return D3D11_BLEND_OP_ADD;
			case BlendOperation::Subtract:				return D3D11_BLEND_OP_SUBTRACT;
			case BlendOperation::ReverseSubtract:		return D3D11_BLEND_OP_REV_SUBTRACT;
			case BlendOperation::Min:					return D3D11_BLEND_OP_MIN;
			case BlendOperation::Max:					return D3D11_BLEND_OP_MAX;
			default:									return D3D11_BLEND_OP_ADD;
			}
		}

		static D3D11_BLEND GetBlendFactor(BlendType factor)
		{
			switch (factor)
			{
			case BlendType::Zero:						return D3D11_BLEND_ZERO;
			case BlendType::One:						return D3D11_BLEND_ONE;
			case BlendType::SrcColor:					return D3D11_BLEND_SRC_COLOR;
			case BlendType::InvSrcColor:				return D3D11_BLEND_INV_SRC_COLOR;
			case BlendType::SrcAlpha:					return D3D11_BLEND_SRC_ALPHA;
			case BlendType::InvSrcAlpha:				return D3D11_BLEND_INV_SRC_ALPHA;
			case BlendType::DestAlpha:					return D3D11_BLEND_DEST_ALPHA;
			case BlendType::InvDestAlpha:				return D3D11_BLEND_INV_DEST_ALPHA;
			case BlendType::DestColor:					return D3D11_BLEND_DEST_COLOR;
			case BlendType::InvDestColor:				return D3D11_BLEND_INV_DEST_COLOR;
			case BlendType::SrcAlphaSat:				return D3D11_BLEND_SRC_ALPHA_SAT;
			case BlendType::BlendFactor:				return D3D11_BLEND_BLEND_FACTOR;
			case BlendType::InvBlendFactor:				return D3D11_BLEND_INV_BLEND_FACTOR;
			case BlendType::Src1Color:					return D3D11_BLEND_SRC1_COLOR;
			case BlendType::InvSrc1Color:				return D3D11_BLEND_INV_SRC1_COLOR;
			case BlendType::Src1Alpha:					return D3D11_BLEND_SRC1_ALPHA;
			case BlendType::InvSrc1Alpha:				return D3D11_BLEND_INV_SRC1_ALPHA;
			default:									return D3D11_BLEND_ZERO;
			}
		}

		static D3D11_COLOR_WRITE_ENABLE GetColorWriteMask(ColorMask mask)
		{
			switch (mask)
			{
			case ColorMask::Red:						return D3D11_COLOR_WRITE_ENABLE_RED;
			case ColorMask::Green:						return D3D11_COLOR_WRITE_ENABLE_GREEN;
			case ColorMask::Blue:						return D3D11_COLOR_WRITE_ENABLE_BLUE;
			case ColorMask::Alpha:						return D3D11_COLOR_WRITE_ENABLE_ALPHA;
			case ColorMask::All:						return D3D11_COLOR_WRITE_ENABLE_ALL;
			default:									return D3D11_COLOR_WRITE_ENABLE_ALL;
			}
		}

		static D3D11_DEPTH_WRITE_MASK GetDepthWriteMask(DepthMask mask)
		{
			switch (mask)
			{
			case DepthMask::Zero:						return D3D11_DEPTH_WRITE_MASK_ZERO;
			case DepthMask::All:						return D3D11_DEPTH_WRITE_MASK_ALL;
			default:									return D3D11_DEPTH_WRITE_MASK_ALL;
			}
		}

		static D3D11_COMPARISON_FUNC GetComparisonFunc(ComparisonFunc func)
		{
			switch (func)
			{
			case ComparisonFunc::Never:			return D3D11_COMPARISON_NEVER;
			case ComparisonFunc::Less:			return D3D11_COMPARISON_LESS;
			case ComparisonFunc::Equal:			return D3D11_COMPARISON_EQUAL;
			case ComparisonFunc::LessEqual:		return D3D11_COMPARISON_LESS_EQUAL;
			case ComparisonFunc::Greater:		return D3D11_COMPARISON_GREATER;
			case ComparisonFunc::NotEqual:		return D3D11_COMPARISON_NOT_EQUAL;
			case ComparisonFunc::GreaterEqual:	return D3D11_COMPARISON_GREATER_EQUAL;
			case ComparisonFunc::Always:		return D3D11_COMPARISON_ALWAYS;
			default:										return D3D11_COMPARISON_ALWAYS;
			}
		}

		static D3D11_INPUT_CLASSIFICATION GetInputClassification(SlotClass classification)
		{
			switch (classification)
			{
			case SlotClass::PerVertexData:					return D3D11_INPUT_PER_VERTEX_DATA;
			case SlotClass::PerInstanceData:				return D3D11_INPUT_PER_INSTANCE_DATA;
			default:										return D3D11_INPUT_PER_VERTEX_DATA;
			}
		}

		static D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(PrimitiveMode topology)
		{
			switch (topology)
			{
			case PrimitiveMode::PointList:			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			case PrimitiveMode::LineList:			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			case PrimitiveMode::LineStrip:			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case PrimitiveMode::TriangleList:		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveMode::TriangleStrip:		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			default:								return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			}
		}

		static D3D11_FILL_MODE GetFillMode(FillMode mode)
		{
			switch (mode)
			{
			case FillMode::Wireframe:				return D3D11_FILL_WIREFRAME;
			case FillMode::Solid:					return D3D11_FILL_SOLID;
			default:								return D3D11_FILL_SOLID;
			}
		}

		static D3D11_CULL_MODE GetCullMode(CullMode mode)
		{
			switch (mode)
			{
			case CullMode::None:					return D3D11_CULL_NONE;
			case CullMode::Front:					return D3D11_CULL_FRONT;
			case CullMode::Back:					return D3D11_CULL_BACK;
			default:								return D3D11_CULL_NONE;
			}
		}

		static bool GetFaceOrdering(FaceOrdering ordering)
		{
			switch (ordering)
			{
			case FaceOrdering::Clockwise:			return false;
			case FaceOrdering::CounterClockwise:	return true;
			default:								return false;
			}
		}

	public:
		D3D11PipelineUtils() = delete;
		~D3D11PipelineUtils() = delete;
	};
}
