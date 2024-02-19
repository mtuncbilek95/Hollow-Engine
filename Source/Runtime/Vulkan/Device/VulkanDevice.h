#pragma once

#include <Runtime/Core/Core.h>
#include <Runtime/Graphics/Device/GraphicsDevice.h>
#include <Runtime/Vulkan/Device/VulkanDeviceDesc.h>

#include <vulkan.h>

namespace Hollow
{
	class RUNTIME_API VulkanDevice : public GraphicsDevice
	{
		struct QueueFamily
		{
			QueueFamily() : QueueCapacity(0), FamilyIndex(255), RequestedCount(0)
			{
			}

			VkQueue GetAvailableQueue()
			{
				if (FreeQueues.size() > 0)
				{
					VkQueue pQ = FreeQueues[0];
					FreeQueues.erase(FreeQueues.begin());
					return pQ;
				}

				return VK_NULL_HANDLE;
			}

			bool HasFreeQueue(VkQueue& qOut)
			{
				if (FreeQueues.size() > 0)
				{
					qOut = FreeQueues[0];
					FreeQueues.erase(FreeQueues.begin());
					return true;
				}

				qOut = VK_NULL_HANDLE;
				return false;
			}

			byte QueueCapacity;
			byte RequestedCount;
			byte FamilyIndex;
			Array<VkQueue> FreeQueues;
		};
	public:
		VulkanDevice(const GraphicsDeviceDesc& desc);
		~VulkanDevice() override = default;

		const VkDevice& GetVkLogicalDevice() const noexcept { return mVkLogicalDevice; }
		const VkPhysicalDevice& GetVkPhysicalDevice() const noexcept { return mVkPhysicalDevice; }

		int32 CatchQueueFamilyIndex(const GraphicsQueueType type);
		VkQueue CatchGraphicsQueue(const GraphicsQueueType type);

		virtual void OnShutdown() override;
	public:
		static PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
		static PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainCore(const SwapchainDesc& desc) override;
		virtual SharedPtr<Shader> CreateShaderCore(const ShaderDesc& desc) override;
		virtual SharedPtr<GraphicsBuffer>CreateGraphicsBufferCore(const GraphicsBufferDesc& desc) override;
		virtual SharedPtr<Texture> CreateTextureCore(const TextureDesc& desc) override;
		virtual SharedPtr<TextureView> CreateTextureViewCore(const TextureViewDesc& desc) override;
		virtual SharedPtr<Sampler> CreateSamplerCore(const SamplerDesc& desc) override;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) override;
		virtual SharedPtr<Pipeline> CreateComputePipelineCore(const ComputePipelineDesc& desc) override;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryCore(const GraphicsMemoryDesc& desc) override;
		virtual SharedPtr<RenderPass> CreateRenderPassCore(const RenderPassDesc& desc) override;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferCore(const CommandBufferDesc& desc) override;
		virtual SharedPtr<CommandPool> CreateCommandPoolCore(const CommandPoolDesc& desc) override;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetCore(const DescriptorSetDesc& desc) override;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolCore(const DescriptorPoolDesc& desc) override;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutCore(const DescriptorLayoutDesc& desc) override;
		//virtual SharedPtr<Fence> CreateFenceCore(const FenceDesc& desc) override;
		virtual SharedPtr<GraphicsQueue> BorrowGraphicsQueueCore(const GraphicsQueueDesc& desc) override;

		//virtual void WaitForFenceCore(Fence** ppFences, byte amount) override;
		//virtual void WaitForIdleDeviceCore() override;
		//virtual void WaitQueueDefaultCore(const GraphicsQueueType type) override;
		virtual void UpdateCPUBufferCore(GraphicsBuffer** buffer, const GraphicsBufferUpdateDesc& desc) override;
		//virtual void UpdateDescriptorSetCore(DescriptorSet** descriptorSet, const DescriptorSetUpdateDesc& desc) override;
		//virtual void SubmitCommandBuffersCore(CommandBuffer** commandBuffers, const byte amount, const GraphicsQueueType type, const Fence* pFence) override;

	private:
		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
		VkDevice mVkLogicalDevice;
		VkPhysicalDevice mVkPhysicalDevice;
	};
}
