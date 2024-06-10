#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDevice final : public GraphicsDevice
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

			void FillQueues(VkDevice vkDevice)
			{
				for (u32 i = 0; i < QueueCapacity; ++i)
				{
					VkQueue pQueue;
					vkGetDeviceQueue(vkDevice, FamilyIndex, i, &pQueue);
					FreeQueues.push_back(pQueue);
				}
			}

			byte QueueCapacity;
			byte FamilyIndex;
			byte RequestedCount;
			ArrayList<VkQueue> FreeQueues;
		};

	public:
		VulkanDevice(const GraphicsDeviceDesc& desc);
		~VulkanDevice() override = default;

		SharedPtr<Texture> CreateTextureForSwapchain(const TextureDesc& desc, VkImage image);

		u32 GetQueueFamilyIndex(GraphicsQueueType type) const;

		VkDevice GetVkDevice() const { return mVkDevice; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkInstance GetVkInstance() const { return mVkInstance; }

		void OnShutdown() override;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) override;
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc) override;
		virtual SharedPtr<TextureBuffer> CreateTextureBufferImpl(const TextureBufferDesc& desc) override;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) override;
		virtual SharedPtr<Semaphore> CreateSyncSemaphoreImpl() override;
		virtual SharedPtr<Fence> CreateSyncFenceImpl(const FenceDesc& desc) override;
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) override;
		virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) override;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) override;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) override;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) override;
		virtual SharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) override;
		virtual SharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc) override;
		virtual SharedPtr<Framebuffer> CreateFramebufferImpl(const FramebufferDesc& desc) override;
		virtual SharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) override;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) override;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) override;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) override;

		virtual void WaitForSemaphoreImpl(SharedPtr<Semaphore> ppSemaphores[], u32 amount) override;
		virtual void WaitForFenceImpl(SharedPtr<Fence> ppFences[], u32 amount) override;
		virtual void ResetFencesImpl(SharedPtr<Fence> ppFences[], u32 amount) override;
		virtual void UpdateBufferDataImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc) override;
		virtual void WaitForIdleImpl() override;
		virtual void WaitQueueIdleImpl(SharedPtr<GraphicsQueue> pQueue) override;
		virtual void SubmitToQueueImpl(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], u32 amount, SharedPtr<Semaphore> ppWaitSemaphores[],
			u32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], u32 signalSemaphoreCount, SharedPtr<Fence> pFence) override;
		virtual void CopyDescriptorSetImpl(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc) override;
		virtual void UpdateDescriptorSetImpl(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc) override;

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkInstance mVkInstance;

		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}
