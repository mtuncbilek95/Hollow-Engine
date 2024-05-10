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
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetCopyDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetUpdateDesc.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

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
	class DescriptorSet;
	class DescriptorPool;
	class DescriptorLayout;

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

		sharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		sharedPtr<GraphicsQueue> CreateGraphicsQueue(const GraphicsQueueDesc& desc);
		sharedPtr<Texture> CreateTexture(const TextureDesc& desc);
		sharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		sharedPtr<Semaphore> CreateSyncSemaphore();
		sharedPtr<Fence> CreateFence(const FenceDesc& desc);
		sharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		sharedPtr<RenderPass> CreateRenderPass(const RenderPassDesc& desc);
		sharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		sharedPtr<CommandBuffer> CreateCommandBuffer(const CommandBufferDesc& desc);
		sharedPtr<CommandPool> CreateCommandPool(const CommandPoolDesc& desc);
		sharedPtr<GraphicsMemory> CreateGraphicsMemory(const GraphicsMemoryDesc& desc);
		sharedPtr<GraphicsBuffer> CreateGraphicsBuffer(const GraphicsBufferDesc& desc);
		sharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		sharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		sharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);
		sharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);

		FORCEINLINE GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

		Swapchain* GetSwapchain() const { return mSwapchain.get(); }
		GraphicsInstance* GetGraphicsInstance() const { return mGraphicsInstance; }
		const GraphicsAdapter& GetGraphicsAdapter() const { return mGraphicsAdapter; }

		void WaitForSemaphore(Semaphore** ppSemaphores, uint32 amount);
		void WaitForFence(Fence** ppFences, uint32 amount);
		void ResetFences(Fence** ppFences, uint32 amount);
		void UpdateBufferData(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc);
		void WaitForIdle();
		void WaitQueueIdle(GraphicsQueue* pQueue);
		void SubmitToQueue(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores,
			uint32 waitSemaphoreCount, PipelineStageFlags* stageFlags, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence);
		void CopyDescriptorSet(DescriptorSet* pSrcSet, DescriptorSet* pDstSet, DescriptorSetCopyDesc& desc);
		void UpdateDescriptorSet(DescriptorSet* pDstSet, DescriptorSetUpdateDesc& desc);

	protected:
		virtual sharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual sharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc) = 0;
		virtual sharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc) = 0;
		virtual sharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) = 0;
		virtual sharedPtr<Semaphore> CreateSyncSemaphoreImpl() = 0;
		virtual sharedPtr<Fence> CreateFenceImpl(const FenceDesc& desc) = 0;
		virtual sharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		virtual sharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc) = 0;
		virtual sharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		virtual sharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) = 0;
		virtual sharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) = 0;
		virtual sharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		virtual sharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) = 0;
		virtual sharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) = 0;
		virtual sharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) = 0;
		virtual sharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) = 0;
		virtual sharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) = 0;

		virtual void WaitForSemaphoreImpl(Semaphore** ppSemaphores, uint32 amount) = 0;
		virtual void WaitForFenceImpl(Fence** ppFences, uint32 amount) = 0;
		virtual void ResetFencesImpl(Fence** ppFences, uint32 amount) = 0;
		virtual void UpdateBufferDataImpl(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc) = 0;
		virtual void WaitForIdleImpl() = 0;
		virtual void WaitQueueIdleImpl(GraphicsQueue* pQueue) = 0;
		virtual void SubmitToQueueImpl(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores,
			uint32 waitSemaphoreCount, PipelineStageFlags* stageFlags, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence) = 0;
		virtual void CopyDescriptorSetImpl(DescriptorSet* pSrcSet, DescriptorSet* pDstSet, DescriptorSetCopyDesc& desc) = 0;
		virtual void UpdateDescriptorSetImpl(DescriptorSet* pDstSet, DescriptorSetUpdateDesc& desc) = 0;

	protected:
		arrayList<sharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		sharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;

		GraphicsInstance* mGraphicsInstance;
		GraphicsAdapter mGraphicsAdapter;
	};
}
