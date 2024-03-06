#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>

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

		//SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);

		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

	protected:
		Array<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		Swapchain* mSwapchain;
		GraphicsAPI mGraphicsAPI;

		GraphicsInstance* mGraphicsInstance;
		GraphicsAdapter mGraphicsAdapter;
	};
}
