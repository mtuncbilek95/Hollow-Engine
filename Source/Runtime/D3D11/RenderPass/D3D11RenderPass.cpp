#include "D3D11RenderPass.h"

#include <Runtime/D3D11/Device/D3D11Device.h>
#include <Runtime/D3D11/Swapchain/D3D11Swapchain.h>

#include <Runtime/D3D11/Texture/D3D11TextureUtils.h>
#include <Runtime/D3D11/Texture/D3D11Texture.h>

namespace Hollow
{
	D3D11RenderPass::D3D11RenderPass(const RenderPassDesc& desc, D3D11Device* pDevice) : RenderPass(desc)
	{
		// I will use one rtv for now. I may change this later
		// Reserve space for the rtv
		mD3DRenderTargetViews.resize(desc.ColorAttachments.size());

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = D3D11TextureUtils::GetDXTextureFormat(desc.ColorAttachments[0].pTexture->GetFormat());

		if (std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount() > 1)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = desc.ColorAttachments[0].MipLevel;
		}

		ID3D11Texture2D* pTexture = std::dynamic_pointer_cast<D3D11Texture>(desc.ColorAttachments[0].pTexture)->GetD3DTexture().Get();

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateRenderTargetView(pTexture,
			&rtvDesc, mD3DRenderTargetViews[0].GetAddressOf())), "D3D11RenderPass", "Failed to create Render Target View");

		// Create depth stencil view

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = D3D11TextureUtils::GetDXTextureFormat(desc.DepthStencilAttachment.pTexture->GetFormat());

		if (std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount() > 1)
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			dsvDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
		}
		else
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsvDesc.Texture2D.MipSlice = desc.DepthStencilAttachment.MipLevel;
		}

		ID3D11Texture2D* pDepthStencilTexture = std::dynamic_pointer_cast<D3D11Texture>(desc.DepthStencilAttachment.pTexture)->GetD3DTexture().Get();

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateDepthStencilView(pDepthStencilTexture, &dsvDesc, &mD3DDepthStencilView)),
			"D3D11RenderPass", "Failed to create Depth Stencil View");

		// Create shader resource view for color attachments

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = D3D11TextureUtils::GetDXTextureFormat(desc.ColorAttachments[0].pTexture->GetFormat());

		if (std::dynamic_pointer_cast<D3D11Swapchain>(pDevice->GetSwapchain())->GetSampleCount() > 1)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			srvDesc.Texture2DMS.UnusedField_NothingToDefine = 0;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.ColorAttachments[0].MipLevel - 1;
			srvDesc.Texture2D.MostDetailedMip = desc.ColorAttachments[0].MipLevel;
		}

		DEV_ASSERT(SUCCEEDED(pDevice->GetD3DDevice()->CreateShaderResourceView(pTexture, &srvDesc, &mD3DShaderResourceView)),
			"D3D11RenderPass", "Failed to create Shader Resource View");

		CORE_LOG(HE_VERBOSE, "D3D11RenderPass", "Operation is successful.");
	}

}
