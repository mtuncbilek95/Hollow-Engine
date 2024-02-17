#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11TextureView : public TextureView
	{
	public:
		D3D11TextureView(const TextureViewDesc& desc, D3D11Device* pDevice);
		~D3D11TextureView() = default;

		ComPtr<ID3D11ShaderResourceView> GetD3DShaderResourceView() const { return mD3DShaderResourceView; }

	private:
		ComPtr<ID3D11ShaderResourceView> mD3DShaderResourceView;
	};
}
