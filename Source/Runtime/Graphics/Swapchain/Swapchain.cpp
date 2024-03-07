#include "Swapchain.h"

#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Graphics/Fence/Fence.h>

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mImageSize(desc.ImageSize), mSwapchainFormat(desc.SwapchainImageFormat), mSwapchainUsage(desc.SwapchainUsage), mBufferCount(desc.BufferCount),
		mGraphicsQueue(desc.pQueue), mCurrentFrameIndex(0), mPresentMode(desc.VSync), mShareMode(desc.SwapchainMode)
	{
		// Create the fences for the swapchain
		for(uint32 i = 0; i < mBufferCount; i++)
		{
			FenceDesc fenceDesc = {};
			fenceDesc.bSignaled = true;

			mFences.push_back(pDevice->CreateFence(fenceDesc));
		}
	}

	void Swapchain::Resize(const Vector2u& newSize)
	{
		mImageSize = newSize;
		ResizeImpl(newSize);
	}

	void Swapchain::Present(Semaphore** ppWaitSemaphores, uint32 amount)
	{
		// Wait for the fence to be signaled
		Fence* pFence = mFences[mCurrentFrameIndex].get();
		GetOwnerDevice()->WaitForFence(&pFence, 1);
		GetOwnerDevice()->ResetFences(&pFence, 1);

		PresentImpl(ppWaitSemaphores, amount);
		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mBufferCount;
	}

	void Swapchain::AddTexture(SharedPtr<Texture> pTexture)
	{
		mImages.push_back(pTexture);
	}

	void Swapchain::AddTextureView(SharedPtr<TextureView> pTextureView)
	{
		mImageViews.push_back(pTextureView);
	}
}