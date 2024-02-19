#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>

namespace Hollow
{
	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
	public:
		Swapchain(const SwapchainDesc& desc);
		virtual ~Swapchain() override = default;

		void Present();
		void Resize(Vector2u newSize);

		Vector2u GetImageSize() const { return mImageSize; }
		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		PresentMode GetPresentMode() const { return mPresentMode; }
		const byte& GetBufferCount() const { return mBufferCount; }
		GraphicsQueue* GetQueue() const { return mGraphicsQueue; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

		virtual void OnShutdown() noexcept override = 0;

	protected:
		virtual void PresentCore() = 0;
		virtual void ResizeCore(Vector2u newSize) = 0;

	private:
		Vector2u mImageSize;
		TextureFormat mSwapchainFormat;
		PresentMode mPresentMode;
		byte mBufferCount;
		GraphicsQueue* mGraphicsQueue;
	};
}
