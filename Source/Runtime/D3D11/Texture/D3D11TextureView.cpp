#include "D3D11TextureView.h"

#include <Runtime/D3D11/Texture/D3D11TextureUtils.h>
#include <Runtime/D3D11/Texture/D3D11Texture.h>

namespace Hollow
{
	D3D11TextureView::D3D11TextureView(const TextureViewDesc& desc, D3D11Device* pDevice) : TextureView(desc)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = D3D11TextureUtils::GetDXTextureFormat(desc.pTexture->GetFormat());
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ID3D11Resource* pTexture = std::dynamic_pointer_cast<D3D11Texture>(desc.pTexture)->GetD3DTexture().Get();
		pDevice->GetD3DDevice()->CreateShaderResourceView(pTexture, &srvDesc, &mD3DShaderResourceView);
	}
}
