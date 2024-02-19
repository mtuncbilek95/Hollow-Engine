#pragma once

#include <Runtime/Core/Core.h>

#include <Runtime/Graphics/Device/GraphicsDeviceDesc.h>
#include <Runtime/Graphics/Adapter/GraphicsAdapterDesc.h>

#include <Runtime/Graphics/Buffer/GraphicsBufferDesc.h>
#include <Runtime/Graphics/Buffer/GraphicsBufferUpdateDesc.h>

#include <Runtime/Graphics/Command/CommandBufferDesc.h>
#include <Runtime/Graphics/Command/CommandPoolDesc.h>

#include <Runtime/Graphics/Memory/GraphicsMemoryDesc.h>

#include <Runtime/Graphics/Pipeline/GraphicsPipelineDesc.h>
#include <Runtime/Graphics/Pipeline/ComputePipelineDesc.h>

#include <Runtime/Graphics/Texture/TextureDesc.h>
#include <Runtime/Graphics/Texture/TextureViewDesc.h>

#include <Runtime/Graphics/Shader/ShaderDesc.h>
#include <Runtime/Graphics/Sampler/SamplerDesc.h>
#include <Runtime/Graphics/Swapchain/SwapchainDesc.h>

#include <Runtime/Graphics/Descriptor/DescriptorSetDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorPoolDesc.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayoutDesc.h>
//#include <Runtime/Graphics/Descriptor/DescriptorSetUpdateDesc.h>

#include <Runtime/Graphics/RenderPass/RenderPassDesc.h>

//#include <Runtime/Graphics/Fence/FenceDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueueDesc.h>
#include <Runtime/Graphics/Queue/GraphicsQueueType.h>

namespace Hollow
{
	class GraphicsAdapter;
	class GraphicsDeviceObject;
	class Swapchain;
	class Shader;
	class GraphicsBuffer;
	class Texture;
	class TextureView;
	class Sampler;
	class Pipeline;
	class GraphicsMemory;
	class DescriptorSet;
	class DescriptorPool;
	class DescriptorLayout;
	class CommandBuffer;
	class CommandPool;
	class RenderPass;
	class GraphicsFence;
	class GraphicsQueue;

	/**
	 * @class GraphicsDevice
	 *
	 * @brief It is a both abstraction layer and usable interface via
	 * higher abstractions. Responsible for creating and managing the
	 * objects that are related to the graphics device.
	 *
	 */
	class RUNTIME_API GraphicsDevice
	{
	public:
		GraphicsDevice(const GraphicsDeviceDesc& desc);
		virtual ~GraphicsDevice() = default;

		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		SharedPtr<GraphicsBuffer> CreateGraphicsBuffer(const GraphicsBufferDesc& desc);
		SharedPtr<Texture> CreateTexture(const TextureDesc& desc);
		SharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);
		SharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<Pipeline> CreateComputePipeline(const ComputePipelineDesc& desc);
		SharedPtr<GraphicsMemory> CreateGraphicsMemory(const GraphicsMemoryDesc& desc);
		SharedPtr<RenderPass> CreateRenderPass(const RenderPassDesc& desc);
		SharedPtr<CommandBuffer> CreateCommandBuffer(const CommandBufferDesc& desc);
		SharedPtr<CommandPool> CreateCommandPool(const CommandPoolDesc& desc);
		SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		SharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		SharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);
		//SharedPtr<GraphicsFence> CreateFence(const GraphicsFenceDesc& desc);
		SharedPtr<GraphicsQueue> BorrowGraphicsQueue(const GraphicsQueueDesc& desc);

		//void WaitForFence(Fence** ppFences, byte amount);
		//void WaitForIdleDevice();
		//void WaitQueueDefault(const GraphicsQueueType type);
		void UpdateCPUBuffer(GraphicsBuffer** buffer, const GraphicsBufferUpdateDesc& desc);
		//void UpdateDescriptorSet(DescriptorSet** descriptorSet, const DescriptorSetUpdateDesc& desc);
		//void SubmitCommandBuffers(CommandBuffer** commandBuffers, const byte amount, const GraphicsQueueType type, const Fence* pFence);

		const SharedPtr<Swapchain>& GetSwapchain() const { return mSwapchain; }
		const SharedPtr<GraphicsAdapter>& GetGraphicsAdapter() const { return mGraphicsAdapter; }

		GraphicsAPI GetGraphicsAPI() const { return mGraphicsAPI; }

		virtual void OnShutdown() = 0;

	protected:
		virtual SharedPtr<Swapchain> CreateSwapchainCore(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<Shader> CreateShaderCore(const ShaderDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer>CreateGraphicsBufferCore(const GraphicsBufferDesc& desc) = 0;
		virtual SharedPtr<Texture> CreateTextureCore(const TextureDesc& desc) = 0;
		virtual SharedPtr<TextureView> CreateTextureViewCore(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Sampler> CreateSamplerCore(const SamplerDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateComputePipelineCore(const ComputePipelineDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateGraphicsMemoryCore(const GraphicsMemoryDesc& desc) = 0;
		virtual SharedPtr<RenderPass> CreateRenderPassCore(const RenderPassDesc& desc) = 0;
		virtual SharedPtr<CommandBuffer> CreateCommandBufferCore(const CommandBufferDesc& desc) = 0;
		virtual SharedPtr<CommandPool> CreateCommandPoolCore(const CommandPoolDesc& desc) = 0;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetCore(const DescriptorSetDesc& desc) = 0;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolCore(const DescriptorPoolDesc& desc) = 0;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutCore(const DescriptorLayoutDesc& desc) = 0;
		//virtual SharedPtr<GraphicsFence> CreateFenceCore(const GraphicsFenceDesc& desc) = 0;
		virtual SharedPtr<GraphicsQueue> BorrowGraphicsQueueCore(const GraphicsQueueDesc& desc) = 0;

		//virtual void WaitForFenceCore(Fence** ppFences, byte amount) = 0;
		//virtual void WaitForIdleDeviceCore() = 0;
		//virtual void WaitQueueDefaultCore(const GraphicsQueueType type) = 0;
		virtual void UpdateCPUBufferCore(GraphicsBuffer** buffer, const GraphicsBufferUpdateDesc& desc) = 0;
		//virtual void UpdateDescriptorSetCore(DescriptorSet** descriptorSet, const DescriptorSetUpdateDesc& desc) = 0;
		//virtual void SubmitCommandBuffersCore(CommandBuffer** commandBuffers, const byte amount, const GraphicsQueueType type, const Fence* pFence) = 0;

	protected:
		Array<SharedPtr<GraphicsDeviceObject>> mDeviceObjects;
		SharedPtr<Swapchain> mSwapchain;
		GraphicsAPI mGraphicsAPI;
		SharedPtr<GraphicsAdapter> mGraphicsAdapter;
	};
}
