#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

#include <Engine/Graphics/Queue/GraphicsQueueDesc.h>
#include <Engine/Graphics/Memory/GraphicsMemoryDesc.h>
#include <Engine/Graphics/Texture/TextureImageDesc.h>
#include <Engine/Graphics/Texture/TextureViewDesc.h>
#include <Engine/Graphics/Sampler/SamplerDesc.h>
#include <Engine/Graphics/Buffer/GraphicsBufferDesc.h>
#include <Engine/Graphics/Shader/ShaderDesc.h>
#include <Engine/Graphics/Swapchain/SwapchainDesc.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutDesc.h>
#include <Engine/Graphics/Descriptor/DescriptorPoolDesc.h>
#include <Engine/Graphics/Descriptor/DescriptorSetDesc.h>
#include <Engine/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Engine/Graphics/Command/CmdPoolDesc.h>
#include <Engine/Graphics/Command/CmdBufferDesc.h>

#include <Engine/Graphics/Pipeline/PipelineStageFlags.h>
#include <Engine/Graphics/Core/BufferDataUpdateDesc.h>
#include <Engine/Graphics/Descriptor/DescriptorUpdateDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsQueue;
	class ENGINE_API GraphicsMemory;
	class ENGINE_API TextureImage;
	class ENGINE_API TextureView;
	class ENGINE_API Sampler;
	class ENGINE_API GraphicsBuffer;
	class ENGINE_API Shader;
	class ENGINE_API Swapchain;
	class ENGINE_API DescriptorLayout;
	class ENGINE_API DescriptorPool;
	class ENGINE_API DescriptorSet;
	class ENGINE_API Pipeline;
	class ENGINE_API Fence;
	class ENGINE_API Semaphore;
	class ENGINE_API CmdPool;
	class ENGINE_API CmdBuffer;

	class ENGINE_API GraphicsInstance;
	class ENGINE_API GraphicsDevice : public IObject
	{
		using WeakInstance = WeakPtr<GraphicsInstance>;

	public:
		GraphicsDevice(WeakInstance pInstance);
		virtual ~GraphicsDevice() override = default;

		WeakInstance GetInstance() const { return mInstance; }

	public:
		SharedPtr<GraphicsQueue> CreateQueue(const GraphicsQueueDesc& desc);
		SharedPtr<GraphicsMemory> CreateMemory(const GraphicsMemoryDesc& desc);
		SharedPtr<TextureImage> CreateTextureImage(const TextureImageDesc& desc);
		SharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);
		SharedPtr<GraphicsBuffer> CreateBuffer(const GraphicsBufferDesc& desc);
		SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);
		SharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		SharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<Fence> CreateGraphicsFence(bool bSignalled);
		SharedPtr<Semaphore> CreateGraphicsSemaphore();
		SharedPtr<CmdPool> CreateCommandPool(const CmdPoolDesc& desc);
		SharedPtr<CmdBuffer> CreateCommandBuffer(const CmdBufferDesc& desc);

		void WaitFences(WeakPtr<Fence> pFences[], u32 count);
		void WaitFence(WeakPtr<Fence> pFence);
		void ResetFences(WeakPtr<Fence> pFences[], u32 count);
		void ResetFence(WeakPtr<Fence> pFence);

		void WaitIdle();
		void WaitQueueIdle(WeakPtr<GraphicsQueue> pQueue);
		void SubmitQueue(WeakPtr<GraphicsQueue> pQueue, WeakPtr<CmdBuffer> pCmdBuffer, u32 cmdCount, 
			WeakPtr<Semaphore> pWaitSemaphores[], u32 waitCount, WeakPtr<Semaphore> pSignalSemaphores[], 
			u32 signalCount, WeakPtr<Fence> pFence, PipelineStageFlags flags[]);

		void UpdateHostBuffer(WeakPtr<GraphicsBuffer> pBuffer, const BufferDataUpdateDesc& desc);
		void UpdateDescriptorSet(WeakPtr<DescriptorSet> pDescriptorSet, const DescriptorUpdateDesc& desc);


	protected:
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		virtual SharedPtr<TextureImage> CreateTextureImageImpl(const TextureImageDesc& desc) = 0;
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer> CreateBufferImpl(const GraphicsBufferDesc& desc) = 0;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) = 0;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) = 0;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<Fence> CreateGraphicsFenceImpl(bool bSignalled) = 0;
		virtual SharedPtr<Semaphore> CreateGraphicsSemaphoreImpl() = 0;
		virtual SharedPtr<CmdPool> CreateCommandPoolImpl(const CmdPoolDesc& desc) = 0;
		virtual SharedPtr<CmdBuffer> CreateCommandBufferImpl(const CmdBufferDesc& desc) = 0;

		virtual void WaitFencesImpl(WeakPtr<Fence> pFences[], u32 count) = 0;
		virtual void WaitFenceImpl(WeakPtr<Fence> pFence) = 0;
		virtual void ResetFencesImpl(WeakPtr<Fence> pFences[], u32 count) = 0;
		virtual void ResetFenceImpl(WeakPtr<Fence> pFence) = 0;

		virtual void WaitIdleImpl() = 0;
		virtual void WaitQueueIdleImpl(WeakPtr<GraphicsQueue> pQueue) = 0;
		virtual void SubmitQueueImpl(WeakPtr<GraphicsQueue> pQueue, WeakPtr<CmdBuffer> pCmdBuffer, u32 cmdCount, 
			WeakPtr<Semaphore> pWaitSemaphores[], u32 waitCount, WeakPtr<Semaphore> pSignalSemaphores[], 
			u32 signalCount, WeakPtr<Fence> pFence, PipelineStageFlags flags[]) = 0;

		virtual void UpdateHostBufferImpl(WeakPtr<GraphicsBuffer> pBuffer, const BufferDataUpdateDesc& desc) = 0;
		virtual void UpdateDescriptorSetImpl(WeakPtr<DescriptorSet> pDescriptorSet, const DescriptorUpdateDesc& desc) = 0;

	private:
		WeakInstance mInstance;
	};
}
