#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>

namespace Hollow
{
	class RUNTIME_API Texture;
	class RUNTIME_API TextureBuffer;
	class RUNTIME_API Fence;
	class RUNTIME_API Semaphore;

	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
		friend class VulkanSwapchain;
	public:
		Swapchain(const SwapchainDesc& desc, const SharedPtr<GraphicsDevice> pDevice);
		virtual ~Swapchain() override = default;

		void Resize(const Vec2u& newSize);
		void AcquireNextImage(SharedPtr<Fence> fence);
		void Present();

		Vec2u GetImageSize() const { return mImageSize; }
		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		TextureUsage GetSwapchainUsage() const { return mSwapchainUsage; }
		PresentMode GetPresentMode() const { return mPresentMode; }
		byte GetBufferCount() const { return mBufferCount; }
		SharedPtr<GraphicsQueue> GetPresentQueue() const { return mGraphicsQueue; }
		u32 GetCurrentFrameIndex() const { return mCurrentFrameIndex; }
		ShareMode GetShareMode() const { return mShareMode; }

		const DArray<SharedPtr<Texture>>& GetImages() const { return mImages; }
		SharedPtr<Texture> GetImage(u32 index) const { return mImages[index]; }
		const DArray<SharedPtr<TextureBuffer>>& GetImageViews() const { return mImageBuffers; }
		SharedPtr<TextureBuffer> GetImageView(u32 index) const { return mImageBuffers[index]; }

		SharedPtr<Semaphore> GetImageSemaphore(u32 index) const { return mImageSemaphores[index]; }
		DArray<SharedPtr<Semaphore>>& GetImageSemaphores() { return mImageSemaphores; }
		SharedPtr<Semaphore> GetFlightSemaphore(u32 index) const { return mFlightSemaphores[index]; }
		DArray<SharedPtr<Semaphore>>& GetFlightSemaphores() { return mFlightSemaphores; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

	protected:
		void SetNewImageSize(const Vec2u newSize) { mImageSize = newSize; }

		void AddTexture(SharedPtr<Texture> pTexture);
		void AddTextureBuffer(SharedPtr<TextureBuffer> pTextureView);

		virtual void ResizeImpl(const Vec2u& newSize) = 0;
		virtual void AcquireNextImageImpl(SharedPtr<Fence> fence) = 0;
		virtual void PresentImpl() = 0;

	private:
		byte mBufferCount;
		Vec2u mImageSize;
		TextureFormat mSwapchainFormat;
		TextureUsage mSwapchainUsage;
		SharedPtr<GraphicsQueue> mGraphicsQueue;
		PresentMode mPresentMode;
		ShareMode mShareMode;
		u32 mCurrentFrameIndex;
		DArray<SharedPtr<Texture>> mImages;
		DArray<SharedPtr<TextureBuffer>> mImageBuffers;
		DArray<SharedPtr<Semaphore>> mImageSemaphores;
		DArray<SharedPtr<Semaphore>> mFlightSemaphores;
	};
}