#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDevice : public GraphicsDevice
	{
		struct QueueFamily
		{
			QueueFamily() : QueueCapacity(0), FamilyIndex(255), RequestedCount(0)
			{}

			bool HasFreeQueue() const { return FreeQueues.size() > 0; }

			VkQueue GetFreeQueue()
			{
				if (FreeQueues.size() > 0)
				{
					VkQueue pQueue = FreeQueues[0];
					FreeQueues.erase(FreeQueues.begin());
					return pQueue;
				}
				return VK_NULL_HANDLE;
			}

			byte QueueCapacity;
			byte FamilyIndex;
			byte RequestedCount;
			arrayList<VkQueue> FreeQueues;
		};

	public:
		VulkanDevice(const GraphicsDeviceDesc& desc);
		~VulkanDevice() override = default;

		sharedPtr<Texture> CreateTextureForSwapchain(const TextureDesc& desc, VkImage image);

		uint32 GetQueueFamilyIndex(GraphicsQueueType type) const;

		VkDevice GetVkDevice() const { return mVkDevice; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkInstance GetVkInstance() const { return mVkInstance; }

		virtual void OnShutdown() override;

	protected:
		virtual sharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) override;
		virtual sharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc)  override;
		virtual sharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc)  override;
		virtual sharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) override;
		virtual sharedPtr<Semaphore> CreateSyncSemaphoreImpl() override;
		virtual sharedPtr<Fence> CreateFenceImpl(const FenceDesc& desc) override;
		virtual sharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) override;
		virtual sharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc) override;
		virtual sharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) override;
		virtual sharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) override;
		virtual sharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) override;
		virtual sharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) override;
		virtual sharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) override;
		virtual sharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) override;
		virtual sharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) override;
		virtual sharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) override;
		virtual sharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) override;

		virtual void WaitForSemaphoreImpl(Semaphore** ppSemaphores, uint32 amount) override;
		virtual void WaitForFenceImpl(Fence** ppFences, uint32 amount) override;
		virtual void ResetFencesImpl(Fence** ppFences, uint32 amount) override;
		virtual void UpdateBufferDataImpl(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc) override;
		virtual void WaitForIdleImpl() override;
		virtual void WaitQueueIdleImpl(GraphicsQueue* pQueue) override;
		virtual void SubmitToQueueImpl(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores, 
			uint32 waitSemaphoreCount, PipelineStageFlags* stageFlags, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence) override;
		virtual void CopyDescriptorSetImpl(DescriptorSet* pSrcSet, DescriptorSet* pDstSet, DescriptorSetCopyDesc& desc) override;
		virtual void UpdateDescriptorSetImpl(DescriptorSet* pDstSet, DescriptorSetUpdateDesc& desc) override;

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkInstance mVkInstance;

		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}