#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Swapchain/SwapchainDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsQueue;
	class ENGINE_API Fence;
	class ENGINE_API Semaphore;
	class ENGINE_API GraphicsDevice;
	class ENGINE_API TextureImage;
	class ENGINE_API TextureView;
	class ENGINE_API Swapchain : public DeviceObject
	{
	public:
		Swapchain(const SwapchainDesc& desc, WeakPtr<GraphicsDevice> pDevice) : DeviceObject(pDevice),
			ImageSize(desc.ImageSize), ImageFormat(desc.ImageFormat), TextureUsage(desc.TextureUsage),
			VSync(desc.VSync), BufferCount(desc.BufferCount), pRequestQueue(desc.pRequestQueue)
		{}
		virtual ~Swapchain() override = default;

		const Vec2u& GetImageSize() const { return ImageSize; }
		TextureFormat GetImageFormat() const { return ImageFormat; }
		TextureUsageFlags GetTextureUsage() const { return TextureUsage; }
		PresentMode GetVSync() const { return VSync; }
		u8 GetBufferCount() const { return BufferCount; }

		WeakPtr<TextureImage> GetImage(u32 index) const { return mImages[index]; }
		WeakPtr<TextureView> GetImageView(u32 index) const { return mImageViews[index]; }

		u32 AcquireNextImage(SharedPtr<Fence> pFence, SharedPtr<Semaphore> pSemaphore)
		{
			return AcquireNextImageImpl(pFence, pSemaphore);
		}

		void Present(SharedPtr<Semaphore> pSemaphore, u32 indices)
		{
			PresentImpl(pSemaphore, indices);
		}

	protected:
		virtual u32 AcquireNextImageImpl(WeakPtr<Fence> pFence, WeakPtr<Semaphore> pSemaphore) = 0;
		virtual void PresentImpl(WeakPtr<Semaphore> pSemaphore, u32 indices) = 0;

		void SetNewImageSize(Vec2u newSize) { ImageSize = newSize; }

		DArray<SharedPtr<TextureImage>> mImages;
		DArray<SharedPtr<TextureView>> mImageViews;

	private:
		Vec2u ImageSize;
		TextureFormat ImageFormat;
		TextureUsageFlags TextureUsage;
		PresentMode VSync;
		u8 BufferCount;
		WeakPtr<GraphicsQueue> pRequestQueue;
	};
}