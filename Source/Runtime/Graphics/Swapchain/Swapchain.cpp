#include "Swapchain.h"

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc) : mSwapchainFormat(desc.SwapchainFormat), 
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
}
