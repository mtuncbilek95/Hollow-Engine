#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>

namespace Hollow
{
	class Texture;
	class TextureView;
	class Fence;
	class Semaphore;

	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
	public:
		Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice);
		virtual ~Swapchain() override = default;

		void Resize(const Vector2u& newSize);
		void Present(Semaphore** ppWaitSemaphores, uint32 amount);

		Vector2u GetImageSize() const { return mImageSize; }
		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		TextureUsage GetSwapchainUsage() const { return mSwapchainUsage; }
		PresentMode GetPresentMode() const { return mPresentMode; }
		const byte& GetBufferCount() const { return mBufferCount; }
		GraphicsQueue* GetPresentQueue() const { return mGraphicsQueue; }
		const byte& GetCurrentFrameIndex() const { return mCurrentFrameIndex; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		void SetNewImageSize(const Vector2u& newSize) { mImageSize = newSize; }

		void AddTexture(SharedPtr<Texture> pTexture);
		void AddTextureView(SharedPtr<TextureView> pTextureView);
		SharedPtr<Fence> GetFence(uint32 index) const { return mFences[index]; }

		virtual void ResizeImpl(const Vector2u& newSize) = 0;
		virtual void PresentImpl(Semaphore** ppWaitSemaphores, uint32 amount) = 0;

	private:
		byte mBufferCount;
		Vector2u mImageSize;
		TextureFormat mSwapchainFormat;
		TextureUsage mSwapchainUsage;
		GraphicsQueue* mGraphicsQueue;
		PresentMode mPresentMode;
		ShareMode mShareMode;
		byte mCurrentFrameIndex;
		Array<SharedPtr<Texture>> mImages;
		Array<SharedPtr<TextureView>> mImageViews;
		Array<SharedPtr<Fence>> mFences;
	};
}