#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

#include <Engine/Graphics/Queue/GraphicsQueueDesc.h>

namespace Hollow
{
	class ENGINE_API GraphicsQueue;

	class ENGINE_API GraphicsInstance;
	class ENGINE_API GraphicsDevice : public IObject
	{
		using SharedInstance = SharedPtr<GraphicsInstance>;

	public:
		GraphicsDevice(SharedInstance pInstance);
		~GraphicsDevice() override = default;

		SharedInstance GetInstance() const { return mInstance; }

	public:
		SharedPtr<GraphicsQueue> CreateQueue(const GraphicsQueueDesc& desc);
		// SharedPtr<GraphicsMemory> CreateMemory(const GraphicsMemoryDesc& desc);
		// SharedPtr<TextureImage> CreateTextureImage(const TextureImageDesc& desc);
		// SharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		// SharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);
		// SharedPtr<GraphicsBuffer> CreateBuffer(const GraphicsBufferDesc& desc);
		// SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		// SharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);
		// SharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		// SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		// SharedPtr<GraphicsPipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		// SharedPtr<ComputePipeline> CreateComputePipeline(const ComputePipelineDesc& desc);
		// SharedPtr<RayPipeline> CreateRayPipeline(const RayPipelineDesc& desc);
		// SharedPtr<Fence> CreateGraphicsFence(const FenceDesc& desc);
		// SharedPtr<Semaphore> CreateGraphicsSemaphore(const SemaphoreDesc& desc);
		// SharedPtr<CommandPool> CreateCommandPool(const CommandPoolDesc& desc);
		// SharedPtr<CommandBuffer> CreateCommandBuffer(const CommandBufferDesc& desc);

	protected:
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) = 0;
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
		SharedInstance mInstance;
	};
}
