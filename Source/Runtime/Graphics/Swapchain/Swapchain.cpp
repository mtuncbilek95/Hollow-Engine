#include "Swapchain.h"

#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

namespace Hollow
{
	Swapchain::Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : GraphicsDeviceObject(pDevice), mSwapchainFormat(desc.SwapchainFormat),
		mPresentMode(desc.VSync), mBufferCount(desc.BufferCount), mImageSize(desc.ImageSize),
		mGraphicsQueue(desc.pQueue), mCurrentFrameIndex(0)
	{
		mFences.resize(mBufferCount);
		for (size_t i = 0; i < mBufferCount; i++)
		{
			FenceDesc fenceDesc;
			fenceDesc.bSignalled = true;
			mFences[i] = pDevice->CreateFence(fenceDesc);
		}
	}

	void Swapchain::WaitForPresent(byte index)
	{
		// Take mFences to static array
		Fence** fences = new Fence* [mBufferCount];

		for (size_t i = 0; i < mBufferCount; i++)
		{
			fences[i] = mFences[i].get();
		}

		GetOwnerDevice()->WaitForFences(&fences[index], 1);

		delete[] fences;
	}

	void Swapchain::Present(Semaphore** ppSemaphores, uint32 waitSemaphoreCount)
	{
		// Take mFences to static array
		Fence** fences = new Fence * [mBufferCount];

		for (size_t i = 0; i < mBufferCount; i++)
		{
			fences[i] = mFences[i].get();
		}

		GetOwnerDevice()->WaitForFences(&fences[mCurrentFrameIndex], 1);
		GetOwnerDevice()->ResetFences(&fences[mCurrentFrameIndex], 1);

		PresentCore(ppSemaphores, waitSemaphoreCount);

		mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mBufferCount;

		delete[] fences;
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
			viewDesc.Aspects = { TextureAspectFlags::ColorAspect };

			mColorImageViews.push_back(GetOwnerDevice()->CreateTextureView(viewDesc));
		}
	}
}
