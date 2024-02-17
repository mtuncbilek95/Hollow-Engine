#include "D3D11Texture.h"

#include <Runtime/D3D11/Texture/D3D11TextureUtils.h>
#include <Runtime/D3D11/Buffer/D3D11BufferUtils.h>
#include <Runtime/Graphics/Texture/TextureUtils.h>

#include <Runtime/D3D11/Device/D3D11Device.h>
#include <Runtime/D3D11/Swapchain/D3D11Swapchain.h>

namespace Hollow
{
	D3D11Texture::D3D11Texture(const TextureDesc& desc, D3D11Device* pDevice) : Texture(desc)
	{
		switch (desc.Type)
		{
		case TextureType::Texture1D:
		{
			break;
		}
		case TextureType::Texture2D:
		{
			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = desc.ImageSize.x;
			textureDesc.Height = desc.ImageSize.y;
			textureDesc.MipLevels = desc.MipLevels;
			textureDesc.ArraySize = desc.ArraySize;
			textureDesc.Format = D3D11TextureUtils::GetDXTextureFormat(desc.Format);
			textureDesc.BindFlags = D3D11TextureUtils::GetDXTextureUsage(desc.Usage);
			textureDesc.CPUAccessFlags = D3D11BufferUtils::GetBufferCPUAccess(desc.CPUAccess);
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.MiscFlags = 0;

			if (desc.Usage == TextureUsage::RenderTarget)
			{
				textureDesc.SampleDesc.Count = std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount();
				textureDesc.SampleDesc.Quality = std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount() > 1 ? 1 : 0;
				textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

				DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateTexture2D(&textureDesc, nullptr, mD3DTexture.GetAddressOf())), "D3D11Texture",
					"Failed to create D3D11Texture");
			}

			if (desc.Usage == TextureUsage::DepthStencil)
			{
				textureDesc.SampleDesc.Count = std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount();
				textureDesc.SampleDesc.Quality = std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount() > 1 ? 1 : 0;
				DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateTexture2D(&textureDesc, nullptr, mD3DTexture.GetAddressOf())), "D3D11Texture",
					"Failed to create D3D11Texture");
			}

			if (desc.Usage == TextureUsage::ShaderResource)
			{
				D3D11_SUBRESOURCE_DATA pInitialData = {};
				pInitialData.pSysMem = reinterpret_cast<void*>(desc.Data);
				pInitialData.SysMemPitch = desc.ImageSize.x * TextureUtils::GetTextureSize(desc.Format);

				DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateTexture2D(&textureDesc, &pInitialData, mD3DTexture.GetAddressOf())), "D3D11Texture",
					"Failed to create D3D11Texture");
			}

			break;
		}
		case TextureType::Texture3D:
		{
			break;
		}
		case TextureType::TextureCube:
		{
			break;
		}
		default:
			CORE_LOG(HE_WARNING, "D3D11Texture", "Invalid texture type %d", desc.Type);
			break;
		}
	}
}
