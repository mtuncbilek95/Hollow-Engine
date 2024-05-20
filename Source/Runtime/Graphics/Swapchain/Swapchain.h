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
	public:
		Swapchain(const SwapchainDesc& desc, const SharedPtr<GraphicsDevice> pDevice);
		virtual ~Swapchain() override = default;

		void Resize(const Vector2u& newSize);
		void AcquireNextImage();
		void Present();

		Vector2u GetImageSize() const { return mImageSize; }
		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		TextureUsage GetSwapchainUsage() const { return mSwapchainUsage; }
		PresentMode GetPresentMode() const { return mPresentMode; }
		const byte& GetBufferCount() const { return mBufferCount; }
		SharedPtr<GraphicsQueue> GetPresentQueue() const { return mGraphicsQueue; }
		const byte& GetCurrentFrameIndex() const { return mCurrentFrameIndex; }
		ShareMode GetShareMode() const { return mShareMode; }

		const ArrayList<SharedPtr<Texture>>& GetImages() const { return mImages; }
		SharedPtr<Texture> GetImage(uint32 index) const { return mImages[index]; }
		const ArrayList<SharedPtr<TextureBuffer>>& GetImageViews() const { return mImageBuffers; }
		SharedPtr<TextureBuffer> GetImageView(uint32 index) const { return mImageBuffers[index]; }

		SharedPtr<Semaphore> GetImageSemaphore(uint32 index) const { return mImageSemaphores[index]; }
		ArrayList<SharedPtr<Semaphore>>& GetImageSemaphores() { return mImageSemaphores; }
		SharedPtr<Semaphore> GetFlightSemaphore(uint32 index) const { return mFlightSemaphores[index]; }
		ArrayList<SharedPtr<Semaphore>>& GetFlightSemaphores() { return mFlightSemaphores; }

		FORCEINLINE GraphicsDeviceObjectType GetObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		void SetNewImageSize(const Vector2u newSize) { mImageSize = newSize; }

		void AddTexture(SharedPtr<Texture> pTexture);
		void AddTextureBuffer(SharedPtr<TextureBuffer> pTextureView);

		virtual void ResizeImpl(const Vector2u& newSize) = 0;
		virtual void AcquireNextImageImpl() = 0;
		virtual void PresentImpl() = 0;

	private:
		byte mBufferCount;
		Vector2u mImageSize;
		TextureFormat mSwapchainFormat;
		TextureUsage mSwapchainUsage;
		SharedPtr<GraphicsQueue> mGraphicsQueue;
		PresentMode mPresentMode;
		ShareMode mShareMode;
		byte mCurrentFrameIndex;
		ArrayList<SharedPtr<Texture>> mImages;
		ArrayList<SharedPtr<TextureBuffer>> mImageBuffers;
		ArrayList<SharedPtr<Semaphore>> mImageSemaphores;
		ArrayList<SharedPtr<Semaphore>> mFlightSemaphores;
	};
}