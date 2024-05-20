#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Object/Object.h>

#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>
#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Texture/TextureBufferDesc.h>
#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>
#include <Runtime/Graphics/Fence/FenceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>
#include <Runtime/Graphics/Shader/ShaderDesc.h>
#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>

#include <Runtime/Graphics/Core/BufferDataUpdateDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetCopyDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorSetUpdateDesc.h>
#include <Runtime/Graphics/Pipeline/PipelineStageFlags.h>

namespace Hollow
{
	class RUNTIME_API GraphicsDeviceObject;
	class RUNTIME_API Swapchain;
	class RUNTIME_API Texture;
	class RUNTIME_API TextureBuffer;
	class RUNTIME_API GraphicsMemory;
	class RUNTIME_API Semaphore;
	class RUNTIME_API Fence;
	class RUNTIME_API GraphicsQueue;
	class RUNTIME_API Sampler;
	class RUNTIME_API Shader;
	class RUNTIME_API Pipeline;
	class RUNTIME_API CommandBuffer;
	class RUNTIME_API CommandPool;
	class RUNTIME_API GraphicsBuffer;
	class RUNTIME_API DescriptorSet;
	class RUNTIME_API DescriptorPool;
	class RUNTIME_API DescriptorLayout;

	/**
	 * @class GraphicsDevice
	 * @brief Abstract class for graphics device
	 */
	class RUNTIME_API GraphicsDevice : public Object
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() override = default;

		SharedPtr<GraphicsInstance> GetInstance() const { return mInstance; }
		SharedPtr<GraphicsAdapter> GetAdapter() const { return mAdapter; }
		SharedPtr<Swapchain> GetSwapchain() const { return mOwnedSwapchain; }

		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Texture> CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureBuffer> CreateTextureBuffer(const TextureBufferDesc& desc);
		SharedPtr<GraphicsMemory> CreateGraphicsMemory(const GraphicsMemoryDesc& desc);
		SharedPtr<Semaphore> CreateSyncSemaphore();
		SharedPtr<Fence> CreateSyncFence(const FenceDesc& desc);
		SharedPtr<GraphicsQueue> CreateQueue(const GraphicsQueueDesc& desc);
		SharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);
		SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		SharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<CommandBuffer> CreateCommandBuffer(const CommandBufferDesc& desc);
		SharedPtr<CommandPool> CreateCommandPool(const CommandPoolDesc& desc);
		SharedPtr<GraphicsBuffer> CreateGraphicsBuffer(const GraphicsBufferDesc& desc);
		SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		SharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		SharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);

		void WaitForSemaphore(SharedPtr<Semaphore> ppSemaphores[], uint32 amount);
		void WaitForFence(SharedPtr<Fence> ppFences[], uint32 amount);
		void ResetFences(SharedPtr<Fence> ppFences[], uint32 amount);
		void UpdateBufferData(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc);
		void WaitForIdle();
		void WaitQueueIdle(SharedPtr<GraphicsQueue> pQueue);
		void SubmitToQueue(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], uint32 amount, SharedPtr<Semaphore> ppWaitSemaphores[],
			uint32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], uint32 signalSemaphoreCount, SharedPtr<Fence> pFence);
		void CopyDescriptorSet(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc);
		void UpdateDescriptorSet(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc);

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureBuffer> CreateTextureBufferImpl(const TextureBufferDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		virtual SharedPtr<Semaphore> CreateSyncSemaphoreImpl() = 0;
		virtual SharedPtr<Fence> CreateSyncFenceImpl(const FenceDesc& desc) = 0;
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) = 0;
		virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) = 0;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) = 0;
		virtual SharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) = 0;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) = 0;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) = 0;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) = 0;

		virtual void WaitForSemaphoreImpl(SharedPtr<Semaphore> ppSemaphores[], uint32 amount) = 0;
		virtual void WaitForFenceImpl(SharedPtr<Fence> ppFences[], uint32 amount) = 0;
		virtual void ResetFencesImpl(SharedPtr<Fence> ppFences[], uint32 amount) = 0;
		virtual void UpdateBufferDataImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc) = 0;
		virtual void WaitForIdleImpl() = 0;
		virtual void WaitQueueIdleImpl(SharedPtr<GraphicsQueue> pQueue) = 0;
		virtual void SubmitToQueueImpl(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], uint32 amount, SharedPtr<Semaphore> ppWaitSemaphores[],
			uint32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], uint32 signalSemaphoreCount, SharedPtr<Fence> pFence) = 0;
		virtual void CopyDescriptorSetImpl(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc) = 0;
		virtual void UpdateDescriptorSetImpl(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc) = 0;

	protected:
		ArrayList<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;

		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<GraphicsAdapter> mAdapter;

		SharedPtr<Swapchain> mOwnedSwapchain;

		byte mGraphicsQueueCount;
		byte mComputeQueueCount;
		byte mTransferQueueCount;
	};
}
