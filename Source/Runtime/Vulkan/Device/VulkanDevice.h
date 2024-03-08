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
			Array<VkQueue> FreeQueues;
		};

	public:
		VulkanDevice(const GraphicsDeviceDesc& desc);
		~VulkanDevice() override = default;

		SharedPtr<Texture> CreateTextureForSwapchain(const TextureDesc& desc, VkImage image);

		uint32 GetQueueFamilyIndex(GraphicsQueueType type) const;

		VkDevice GetVkDevice() const { return mVkDevice; }
		VkPhysicalDevice GetVkPhysicalDevice() const { return mVkPhysicalDevice; }
		VkInstance GetVkInstance() const { return mVkInstance; }

		virtual void OnShutdown() override;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) override;
		virtual SharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc)  override;
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc)  override;
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) override;
		virtual SharedPtr<Semaphore> CreateSyncSemaphoreImpl() override;
		virtual SharedPtr<Fence> CreateFenceImpl(const FenceDesc& desc) override;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) override;
		virtual SharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc) override;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) override;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) override;
		virtual SharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) override;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc) override;
		virtual SharedPtr<GraphicsBuffer> CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc) override;

		virtual void WaitForFenceImpl(Fence** ppFences, uint32 amount) override;
		virtual void ResetFencesImpl(Fence** ppFences, uint32 amount) override;
		virtual void UpdateBufferDataImpl(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc) override;
		virtual void WaitForIdleImpl() override;
		virtual void WaitQueueIdleImpl(GraphicsQueue* pQueue) override;
		virtual void SubmitToQueueImpl(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores, 
			uint32 waitSemaphoreCount, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence) override;

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkInstance mVkInstance;

		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}