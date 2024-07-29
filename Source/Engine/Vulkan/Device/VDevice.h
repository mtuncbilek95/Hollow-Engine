#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class VInstance;
	class VDevice : public GraphicsDevice
	{
		using WeakInstance = WeakPtr<VInstance>;

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
				for (u8 i = 0; i < QueueCapacity; ++i)
				{
					VkQueue pQueue;
					vkGetDeviceQueue(vkDevice, FamilyIndex, i, &pQueue);
					FreeQueues.push_back(pQueue);
				}
			}

			u8 QueueCapacity;
			u8 FamilyIndex;
			u8 RequestedCount;
			DArray<VkQueue> FreeQueues;
		};

	public:
		VDevice(WeakInstance pInstance);
		~VDevice() override = default;

		VkDevice GetVkDevice() const { return mDevice; }
		VkInstance GetVkInstance() const { return mInstance; }
		VkPhysicalDevice GetVkAdapter() const { return mAdapter; }

		u32 GetGraphicsQueueFamilyIndex() const { return mGraphicsQueueFamily.FamilyIndex; }
		u32 GetComputeQueueFamilyIndex() const { return mComputeQueueFamily.FamilyIndex; }
		u32 GetTransferQueueFamilyIndex() const { return mTransferQueueFamily.FamilyIndex; }

		SharedPtr<TextureImage> CreateSwapchainImage(const TextureImageDesc& desc, VkImage image);
		SharedPtr<TextureView> CreateSwapchainImageView(const TextureViewDesc& desc);

	protected:
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) override;
		virtual SharedPtr<GraphicsMemory> CreateMemoryImpl(const GraphicsMemoryDesc& desc) override;
		virtual SharedPtr<TextureImage> CreateTextureImageImpl(const TextureImageDesc& desc) override;
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) override;
		virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) override;
		virtual SharedPtr<GraphicsBuffer> CreateBufferImpl(const GraphicsBufferDesc& desc) override;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) override;
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) override;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) override;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) override;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) override;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) override;
		virtual SharedPtr<Fence> CreateGraphicsFenceImpl(bool bSignalled) override;
		virtual SharedPtr<Semaphore> CreateGraphicsSemaphoreImpl() override;
		virtual SharedPtr<CmdPool> CreateCommandPoolImpl(const CmdPoolDesc& desc) override;
		virtual SharedPtr<CmdBuffer> CreateCommandBufferImpl(const CmdBufferDesc& desc) override;

		virtual void WaitFencesImpl(WeakPtr<Fence> pFences[], u32 count) override;
		virtual void WaitFenceImpl(WeakPtr<Fence> pFence) override;
		virtual void ResetFencesImpl(WeakPtr<Fence> pFences[], u32 count) override;
		virtual void ResetFenceImpl(WeakPtr<Fence> pFence) override;

		virtual void WaitIdleImpl() override;
		virtual void WaitQueueIdleImpl(WeakPtr<GraphicsQueue> pQueue) override;
		virtual void SubmitQueueImpl(WeakPtr<GraphicsQueue> pQueue, WeakPtr<CmdBuffer> pCmdBuffer, u32 cmdCount,
			WeakPtr<Semaphore> pWaitSemaphores[], u32 waitCount, WeakPtr<Semaphore> pSignalSemaphores[],
			u32 signalCount, WeakPtr<Fence> pFence, PipelineStageFlags flags[]) override;

		virtual void UpdateHostBufferImpl(WeakPtr<GraphicsBuffer> pBuffer, const BufferDataUpdateDesc& desc) override;
		virtual void UpdateDescriptorSetImpl(WeakPtr<DescriptorSet> pDescriptorSet, const DescriptorUpdateDesc& desc) override;

	private:
		VkDevice mDevice;
		VkInstance mInstance;
		VkPhysicalDevice mAdapter;

	private:
		QueueFamily mGraphicsQueueFamily;
		QueueFamily mComputeQueueFamily;
		QueueFamily mTransferQueueFamily;
	};
}
