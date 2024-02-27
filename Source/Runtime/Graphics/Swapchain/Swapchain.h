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

		void WaitForPresent(byte index);
		void Present(Semaphore** ppSemaphores, uint32 waitSemaphoreCount);
		void Resize(Vector2u newSize);

		Vector2u GetImageSize() const { return mImageSize; }
		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		PresentMode GetPresentMode() const { return mPresentMode; }
		const byte& GetBufferCount() const { return mBufferCount; }
		GraphicsQueue* GetQueue() const { return mGraphicsQueue; }
		const Array<SharedPtr<Texture>>& GetColorImages() const { return mColorImages; }
		const Array<SharedPtr<TextureView>>& GetColorImageViews() const { return mColorImageViews; }
		const Array<SharedPtr<Fence>>& GetFences() const { return mFences; }
		const byte& GetCurrentFrameIndex() const { return mCurrentFrameIndex; }
		SharedPtr<Fence> GetFence(byte index) const { return mFences[index]; }


		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void PresentCore(Semaphore** ppSemaphores, uint32 waitSemaphoreCount) = 0;
		virtual void ResizeCore(Vector2u newSize) = 0;

		void CreateColorImageViews(Array<SharedPtr<Texture>> textures);

	private:
		Vector2u mImageSize;
		TextureFormat mSwapchainFormat;
		PresentMode mPresentMode;
		byte mBufferCount;
		GraphicsQueue* mGraphicsQueue;
		Array<SharedPtr<Texture>> mColorImages;
		Array<SharedPtr<TextureView>> mColorImageViews;
		Array<SharedPtr<Fence>> mFences;
		byte mCurrentFrameIndex;
	};
}
