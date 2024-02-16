#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Sampler/Sampler.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Sampler : public Sampler
	{
	public:
		D3D11Sampler(const SamplerDesc& desc, ID3D11Device* pDevice);
		~D3D11Sampler() override = default;

		const ComPtr<ID3D11SamplerState>& GetD3DSampler() const { return mD3DSampler; }

	private:
		ComPtr<ID3D11SamplerState> mD3DSampler;
	};
}
