#include "D3D11Sampler.h"

#include <Runtime/D3D11/Sampler/D3D11SamplerUtils.h>

namespace Hollow
{
	D3D11Sampler::D3D11Sampler(const SamplerDesc& desc, ID3D11Device* pDevice) : Sampler(desc)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};

		samplerDesc.Filter = D3D11SamplerUtils::GetSamplerFilter(desc.Filter);
		samplerDesc.AddressU = D3D11SamplerUtils::GetAddressMode(desc.AddressU);
		samplerDesc.AddressV = D3D11SamplerUtils::GetAddressMode(desc.AddressV);
		samplerDesc.AddressW = D3D11SamplerUtils::GetAddressMode(desc.AddressW);
		samplerDesc.MipLODBias = desc.MipLODBias;
		samplerDesc.MaxAnisotropy = desc.MaxAnisotropy;
		samplerDesc.ComparisonFunc = D3D11SamplerUtils::GetComparisonFunc(desc.SamplerComparison);
		samplerDesc.BorderColor[0] = desc.BorderColor[0];
		samplerDesc.BorderColor[1] = desc.BorderColor[1];
		samplerDesc.BorderColor[2] = desc.BorderColor[2];
		samplerDesc.BorderColor[3] = desc.BorderColor[3];
		samplerDesc.MinLOD = desc.MinLOD;
		samplerDesc.MaxLOD = desc.MaxLOD;

		DEV_ASSERT(SUCCEEDED(pDevice->CreateSamplerState(&samplerDesc, mD3DSampler.GetAddressOf())), "D3D11Sampler", "Failed to create D3D11Sampler");

		CORE_LOG(HE_VERBOSE, "D3D11Sampler", "Operation is successful.");
	}
}
