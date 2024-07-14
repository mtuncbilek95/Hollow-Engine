#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>

#include <vulkan/vulkan.h>

namespace Hollow
{
	class ENGINE_API VInstance;
	class ENGINE_API VDevice : public GraphicsDevice
	{
		using SharedInstance = SharedPtr<VInstance>;

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
		VDevice(SharedInstance pInstance);
		~VDevice() override = default;

		VkDevice GetVkDevice() const { return mDevice; }
		VkInstance GetVkInstance() const { return mInstance; }
		VkPhysicalDevice GetVkAdapter() const { return mAdapter; }

		void Shutdown() override;

	protected:
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) override;
		// virtual SharedPtr<GraphicsMemory> CreateMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		// virtual SharedPtr<TextureImage> CreateTextureImageImpl(const TextureImageDesc& desc) = 0;
		// virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) = 0;
		// virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) = 0;
		// virtual SharedPtr<GraphicsBuffer> CreateBufferImpl(const GraphicsBufferDesc& desc) = 0;
		// virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		// virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) = 0;
		// virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) = 0;
		// virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) = 0;
		// virtual SharedPtr<GraphicsPipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		// virtual SharedPtr<ComputePipeline> CreateComputePipelineImpl(const ComputePipelineDesc& desc) = 0;
		// virtual SharedPtr<RayPipeline> CreateRayPipelineImpl(const RayPipelineDesc& desc) = 0;
		// virtual SharedPtr<Fence> CreateGraphicsFenceImpl(const FenceDesc& desc) = 0;
		// virtual SharedPtr<Semaphore> CreateGraphicsSemaphoreImpl(const SemaphoreDesc& desc) = 0;
		// virtual SharedPtr<CommandPool> CreateCommandPoolImpl(const CommandPoolDesc& desc) = 0;
		// virtual SharedPtr<CommandBuffer> CreateCommandBufferImpl(const CommandBufferDesc& desc) = 0;

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
