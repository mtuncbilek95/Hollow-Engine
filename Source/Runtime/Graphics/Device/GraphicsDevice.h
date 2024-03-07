#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>

#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>

namespace Hollow
{
	class GraphicsDeviceObject;
	class Swapchain;
	class Shader;
	class GraphicsBuffer;
	class Texture;
	class TextureView;
	class Sampler;
	class Pipeline;
	class GraphicsMemory;
	class DescriptorSet;
	class DescriptorPool;
	class DescriptorLayout;
	class CommandBuffer;
	class CommandPool;
	class RenderPass;
	class Fence;
	class Semaphore;
	class GraphicsQueue;

	/**
	 * @class GraphicsDevice
	 *
	 * @brief It is a both abstraction layer and usable interface via
	 * higher abstractions. Responsible for creating and managing the
	 * objects that are related to the graphics device.
	 *
	 */
	class RUNTIME_API GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() = default;

		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<GraphicsQueue> CreateGraphicsQueue(const GraphicsQueueDesc& desc);

		FORCEINLINE GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

		Swapchain* GetSwapchain() const { return mSwapchain.get(); }
		GraphicsInstance* GetGraphicsInstance() const { return mGraphicsInstance; }
		const GraphicsAdapter& GetGraphicsAdapter() const { return mGraphicsAdapter; }

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc) = 0;

	protected:
		Array<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		SharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;

		GraphicsInstance* mGraphicsInstance;
		GraphicsAdapter mGraphicsAdapter;
	};
}
