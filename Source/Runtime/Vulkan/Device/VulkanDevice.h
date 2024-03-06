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

		virtual void OnShutdown() override;

	public:
		static PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
		static PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc);

	private:
		VkDevice mVkDevice;
		VkPhysicalDevice mVkPhysicalDevice;
		VkInstance mVkInstance;

		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}