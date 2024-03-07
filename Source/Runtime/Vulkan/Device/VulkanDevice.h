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

	public:
		static PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
		static PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc);
		virtual SharedPtr<GraphicsQueue> CreateGraphicsQueueImpl(const GraphicsQueueDesc& desc);
		virtual SharedPtr<Texture> CreateTextureImpl(const TextureDesc& desc);
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc);
		virtual SharedPtr<Semaphore> CreateSyncSemaphoreImpl();
		virtual SharedPtr<Fence> CreateFenceImpl(const FenceDesc& desc);
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc);
		virtual SharedPtr<RenderPass> CreateRenderPassImpl(const RenderPassDesc& desc);
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc);

		virtual void WaitForFenceImpl(Fence** ppFences, uint32 amount) override;
		virtual void ResetFencesImpl(Fence** ppFences, uint32 amount) override;

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkInstance mVkInstance;

		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}