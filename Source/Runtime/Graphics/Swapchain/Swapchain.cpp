#include "Swapchain.h"

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice),
		mImageSize(desc.ImageSize), mSwapchainFormat(desc.SwapchainImageFormat), mSwapchainUsage(desc.SwapchainUsage), mBufferCount(desc.BufferCount),
		mGraphicsQueue(desc.pQueue), mCurrentFrameIndex(0), mPresentMode(desc.VSync), mShareMode(desc.SwapchainMode)
	{
		// Create the fences for the swapchain
		for(uint32 i = 0; i < mBufferCount; i++)
		{
		}
	}

	void Swapchain::Resize(const Vector2u& newSize)
	{
		mImageSize = newSize;
		ResizeImpl(newSize);
	}

	void Swapchain::Present(Semaphore** ppWaitSemaphores, uint32 amount)
	{
		PresentImpl(ppWaitSemaphores, amount);
		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mBufferCount;
	}
}