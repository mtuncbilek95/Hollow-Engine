#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/SamplerFilter.h>
#include <Runtime/Graphics/Sampler/SamplerAddressMode.h>
#include <Runtime/Graphics/Sampler/SamplerBorderColor.h>
#include <Runtime/Graphics/Sampler/SamplerMapMode.h>
#include <Runtime/Graphics/Core/CompareOperation.h>

namespace Hollow
{
	struct RUNTIME_API SamplerDesc
	{
		SamplerFilter MagFilter;
		SamplerFilter MinFilter;
		SamplerAddressMode UAddressMode;
		SamplerAddressMode VAddressMode;
		SamplerAddressMode WAddressMode;
		bool bAnisotropyEnabled;
		uint32 MaxAnisotropy;
		SamplerBorderColor BorderColor;
		bool bCompareEnabled;
		CompareOperation CompareOp;
		SamplerMapMode MapMode;
		float MipLODBias;
		float MinLOD;
		float MaxLOD;
	};
}
