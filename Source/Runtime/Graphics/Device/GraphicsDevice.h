#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>

#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Texture/TextureViewDesc.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>
#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>
#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Runtime/Graphics/Command/BufferDataUpdateDesc.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>

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
		SharedPtr<Texture> CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Semaphore> CreateSyncSemaphore();
		SharedPtr<Fence> CreateFence(const FenceDesc& desc);
		SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		SharedPtr<RenderPass> CreateRenderPass(const RenderPassDesc& desc);
		SharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<CommandBuffer> CreateCommandBuffer(const CommandBufferDesc& desc);
		SharedPtr<CommandPool> CreateCommandPool(const CommandPoolDesc& desc);
		SharedPtr<GraphicsMemory> CreateGraphicsMemory(const GraphicsMemoryDesc& desc);
		SharedPtr<GraphicsBuffer> CreateGraphicsBuffer(const GraphicsBufferDesc& desc);

		FORCEINLINE GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

		Swapchain* GetSwapchain() const { return mSwapchain.get(); }
		GraphicsInstance* GetGraphicsInstance() const { return mGraphicsInstance; }
		const GraphicsAdapter& GetGraphicsAdapter() const { return mGraphicsAdapter; }

		void WaitForFence(Fence** ppFences, uint32 amount);
		void ResetFences(Fence** ppFences, uint32 amount);
		void UpdateBufferData(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc);
		void WaitForIdle();
		void WaitQueueIdle(GraphicsQueue* pQueue);
		void SubmitToQueue(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores, uint32 waitSemaphoreCount, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence);

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc) = 0;
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Semaphore> CreateSyncSemaphoreImpl() = 0;
		virtual SharedPtr<Fence> CreateFenceImpl(const FenceDesc& desc) = 0;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		virtual SharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) = 0;
		virtual SharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) = 0;

		virtual void WaitForFenceImpl(Fence** ppFences, uint32 amount) = 0;
		virtual void ResetFencesImpl(Fence** ppFences, uint32 amount) = 0;
		virtual void UpdateBufferDataImpl(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc) = 0;
		virtual void WaitForIdleImpl() = 0;
		virtual void WaitQueueIdleImpl(GraphicsQueue* pQueue) = 0;
		virtual void SubmitToQueueImpl(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores, uint32 waitSemaphoreCount, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence) = 0;

	protected:
		Array<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		SharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;

		GraphicsInstance* mGraphicsInstance;
		GraphicsAdapter mGraphicsAdapter;
	};
}
