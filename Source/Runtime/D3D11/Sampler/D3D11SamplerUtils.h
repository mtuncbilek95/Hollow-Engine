#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Sampler/AddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerFilter.h>
#include <Runtime/Graphics/Sampler/SamplerComparison.h>

#include <d3d11_4.h>

namespace Hollow
{
	class RUNTIME_API D3D11SamplerUtils
	{
	public:
		static D3D11_FILTER GetSamplerFilter(SamplerFilter filter)
		{
			switch (filter)
			{
			case SamplerFilter::MinMagMipPoint:								return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case SamplerFilter::MinMagPointMipLinear:						return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MinPointMagLinearMipPoint:					return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MinPointMagMipLinear:						return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
			case SamplerFilter::MinLinearMagMipPoint:						return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
			case SamplerFilter::MinLinearMagPointMipLinear:					return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MinMagLinearMipPoint:						return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MinMagMipLinear:							return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case SamplerFilter::Anisotropic:								return D3D11_FILTER_ANISOTROPIC;
			case SamplerFilter::ComparisonMinMagMipPoint:					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			case SamplerFilter::ComparisonMinMagPointMipLinear:				return D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::ComparisonMinPointMagLinearMipPoint:		return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::ComparisonMinPointMagMipLinear:				return D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
			case SamplerFilter::ComparisonMinLinearMagMipPoint:				return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
			case SamplerFilter::ComparisonMinLinearMagPointMipLinear:		return D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::ComparisonMinMagLinearMipPoint:				return D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::ComparisonMinMagMipLinear:					return D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			case SamplerFilter::ComparisonAnisotropic:						return D3D11_FILTER_COMPARISON_ANISOTROPIC;
			case SamplerFilter::MinimumMinMagMipPoint:						return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
			case SamplerFilter::MinimumMinMagPointMipLinear:				return D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MinimumMinPointMagLinearMipPoint:			return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MinimumMinPointMagMipLinear:				return D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
			case SamplerFilter::MinimumMinLinearMagMipPoint:				return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
			case SamplerFilter::MinimumMinLinearMagPointMipLinear:			return D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MinimumMinMagLinearMipPoint:				return D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MinimumMinMagMipLinear:						return D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
			case SamplerFilter::MinimumAnisotropic:							return D3D11_FILTER_MINIMUM_ANISOTROPIC;
			case SamplerFilter::MaximumMinMagMipPoint:						return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
			case SamplerFilter::MaximumMinMagPointMipLinear:				return D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MaximumMinPointMagLinearMipPoint:			return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MaximumMinPointMagMipLinear:				return D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
			case SamplerFilter::MaximumMinLinearMagMipPoint:				return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
			case SamplerFilter::MaximumMinLinearMagPointMipLinear:			return D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			case SamplerFilter::MaximumMinMagLinearMipPoint:				return D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
			case SamplerFilter::MaximumMinMagMipLinear:						return D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
			case SamplerFilter::MaximumAnisotropic:							return D3D11_FILTER_MAXIMUM_ANISOTROPIC;
			default:														return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}

		static D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(AddressMode addressMode)
		{
			switch (addressMode)
			{
			case AddressMode::Wrap:				return D3D11_TEXTURE_ADDRESS_WRAP;
			case AddressMode::Mirror:			return D3D11_TEXTURE_ADDRESS_MIRROR;
			case AddressMode::Clamp:			return D3D11_TEXTURE_ADDRESS_CLAMP;
			case AddressMode::Border:			return D3D11_TEXTURE_ADDRESS_BORDER;
			case AddressMode::MirrorOnce:		return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
			default:							return D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}

		static D3D11_COMPARISON_FUNC GetComparisonFunc(SamplerComparison comparison)
		{
			switch (comparison)
			{
			case SamplerComparison::Never:			return D3D11_COMPARISON_NEVER;
			case SamplerComparison::Less:			return D3D11_COMPARISON_LESS;
			case SamplerComparison::Equal:			return D3D11_COMPARISON_EQUAL;
			case SamplerComparison::Less_Equal:		return D3D11_COMPARISON_LESS_EQUAL;
			case SamplerComparison::Greater:		return D3D11_COMPARISON_GREATER;
			case SamplerComparison::NotEqual:		return D3D11_COMPARISON_NOT_EQUAL;
			case SamplerComparison::Greater_Equal:	return D3D11_COMPARISON_GREATER_EQUAL;
			case SamplerComparison::Always:			return D3D11_COMPARISON_ALWAYS;
			default:								return D3D11_COMPARISON_NEVER;
			}
		}

	public:
		D3D11SamplerUtils() = delete;
		~D3D11SamplerUtils() = delete;
	};
}
