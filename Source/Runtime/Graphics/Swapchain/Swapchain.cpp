#include "Swapchain.h"

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc) : mSwapchainFormat(desc.SwapchainFormat), mVSync(desc.VSync), 
		mWindowed(desc.Windowed), mBufferCount(desc.BufferCount), mSampleCount(desc.SampleCount)
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

	void Swapchain::SetVSync(bool vsync)
	{
		mVSync = vsync;
	}
}
