#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/SamplerFiltering.h>
#include <Runtime/Graphics/Sampler/SamplerAddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerMapMode.h>
#include <Runtime/Graphics/Sampler/SamplerBorderColor.h>

namespace Hollow
{
	struct RUNTIME_API SamplerDesc
	{
        SamplerFiltering MinFilter;
        SamplerFiltering MagFilter;
        SamplerMapMode MipmapMode;
        SamplerAddressMode AddressModeU;
        SamplerAddressMode AddressModeV;
        SamplerAddressMode AddressModeW;
        float MipLodBias;
        float MaxAnisotropy;
        bool bComparisonEnabled;
        CompareOperation CompareOperation;
        float MinLod;
        float MaxLod;
        SamplerBorderColor BorderColor;
	};
}
