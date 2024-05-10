#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>

#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Texture/TextureBufferDesc.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

namespace Hollow
{
	class RUNTIME_API GraphicsDeviceObject;
	class RUNTIME_API Swapchain;
	class RUNTIME_API Texture;
	class RUNTIME_API TextureBuffer;
	class RUNTIME_API GraphicsMemory;

	class RUNTIME_API GraphicsDevice : public Object
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() override = default;

		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Texture> CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureBuffer> CreateTextureBuffer(const TextureBufferDesc& desc);
		SharedPtr<GraphicsMemory> CreateGraphicsMemory(const GraphicsMemoryDesc& desc);

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureBuffer> CreateTextureBufferImpl(const TextureBufferDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) = 0;

	private:
		ArrayList<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<GraphicsAdapter> mAdapter;

		SharedPtr<Swapchain> mOwnedSwapchain;

		byte mGraphicsQueueCount;
		byte mComputeQueueCount;
		byte mTransferQueueCount;
	};
}
