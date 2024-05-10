#include "Swapchain.h"

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Fence/Fence.h>

#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mImageSize(desc.ImageSize), mSwapchainFormat(desc.SwapchainImageFormat), mSwapchainUsage(desc.SwapchainUsage), mBufferCount(desc.BufferCount),
		mGraphicsQueue(desc.pQueue), mCurrentFrameIndex(0), mPresentMode(desc.VSync), mShareMode(desc.SwapchainMode)
	{
		// Create the fences for the swapchain
		for (uint32 i = 0; i < mBufferCount; i++)
		{
			mSemaphores.push_back(pDevice->CreateSyncSemaphore());
		}
	}

	void Swapchain::Resize(const Vector2u& newSize)
	{
		GetOwnerDevice()->WaitQueueIdle(GetPresentQueue());
		GetOwnerDevice()->WaitForIdle();

		for (byte i = 0; i < mBufferCount; i++)
		{
			mImageViews[i]->OnShutdown();
			mImages[i]->OnShutdown();
		}

		mImages.clear();
		mImageViews.clear();

		mImageSize = newSize;
		ResizeImpl(newSize);
	}

	void Swapchain::Present(Semaphore** ppWaitSemaphores, uint32 amount)
	{
		PresentImpl(ppWaitSemaphores, amount);

		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mBufferCount;
	}

	void Swapchain::AddTexture(sharedPtr<Texture> pTexture)
	{
		mImages.push_back(pTexture);
	}

	void Swapchain::AddTextureView(sharedPtr<TextureView> pTextureView)
	{
		mImageViews.push_back(pTextureView);
	}
}