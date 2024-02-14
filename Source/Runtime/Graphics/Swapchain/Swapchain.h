#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceObject.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>


namespace Hollow
{
	class RUNTIME_API Swapchain : public GraphicsDeviceObject
	{
	public:
		Swapchain(const SwapchainDesc& desc);
		virtual ~Swapchain() override = default;

		void Present();
		void Resize(Vector2u newSize);
		void SetVSync(bool vsync);

		TextureFormat GetSwapchainFormat() const { return mSwapchainFormat; }
		bool GetVSync() const { return mVSync; }
		bool GetWindowed() const { return mWindowed; }
		byte GetBufferCount() const { return mBufferCount; }
		byte GetSampleCount() const { return mSampleCount; }

		FORCEINLINE GraphicsDeviceObjectType GetDeviceObjectType() const noexcept final { return GraphicsDeviceObjectType::Swapchain; }

	protected:
		virtual void PresentCore() = 0;
		virtual void ResizeCore(Vector2u newSize) = 0;

	private:
		TextureFormat mSwapchainFormat;
		bool mVSync;
		bool mWindowed;
		byte mBufferCount;
		byte mSampleCount;
	};
}
