#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/D3D11/Device/D3D11Device.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Texture : public Texture
	{
	public:
		D3D11Texture(const TextureDesc& desc, D3D11Device* pDevice);
		~D3D11Texture() override = default;

		ComPtr<ID3D11Texture2D> GetD3DTexture() const { return mD3DTexture; }

	private:
		ComPtr<ID3D11Texture2D> mD3DTexture;
	};
}
