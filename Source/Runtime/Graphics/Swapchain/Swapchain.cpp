#include "Swapchain.h"

#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mSwapchainFormat(desc.SwapchainFormat),
		mPresentMode(desc.VSync), mBufferCount(desc.BufferCount), mImageSize(desc.ImageSize), 
		mGraphicsQueue(desc.pQueue)
	{
	}

	void Swapchain::Present()
	{
		PresentCore();
	}

	void Swapchain::Resize(Vector2u newSize)
	{
		ResizeCore(newSize);
	}

	void Swapchain::CreateColorImageViews(Array<SharedPtr<Texture>> textures)
	{
		mColorImages = textures;

		for (auto& image : mColorImages)
		{
			TextureViewDesc viewDesc;
			viewDesc.ArraySize = 0;
			viewDesc.MipLevels = 0;
			viewDesc.pTexture = image;
			viewDesc.Aspects = { TextureAspect::TextureColor };

			mColorImageViews.push_back(GetOwnerDevice()->CreateTextureView(viewDesc));
		}
	}
}
