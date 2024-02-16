#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Texture/Texture.h>

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Hollow
{
	class RUNTIME_API D3D11Texture : public Texture
	{
	public:
		D3D11Texture(const TextureDesc& desc, ID3D11Device* pDevice);
		~D3D11Texture() override = default;

		ComPtr<ID3D11Texture2D> GetTexture() const { return mD3DTexture; }

	private:
		ComPtr<ID3D11Texture2D> mD3DTexture;
	};
}
