#include "GraphicsDevice.h"

#include <Runtime/Graphics/Manager/GraphicsManager.h>

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Texture/Texture.h>	
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Sampler/Sampler.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : mGraphicsAdapter(desc.Adapter), mGraphicsAPI(desc.API)
	{
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		if (mSwapchain != nullptr)
		{
			return mSwapchain;
		}

		SharedPtr<Swapchain> swapchain = CreateSwapchainCore(desc);
		mSwapchain = swapchain;
		mDeviceObjects.push_back(swapchain);
		return swapchain;
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		SharedPtr<Shader> shader = CreateShaderCore(desc);
		mDeviceObjects.push_back(shader);
		return shader;
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateGraphicsBuffer(const GraphicsBufferDesc& desc)
	{
		SharedPtr<GraphicsBuffer> graphicsBuffer = CreateGraphicsBufferCore(desc);
		mDeviceObjects.push_back(graphicsBuffer);
		return graphicsBuffer;
	}

	SharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		SharedPtr<Texture> texture = CreateTextureCore(desc);
		mDeviceObjects.push_back(texture);
		return texture;
	}

	SharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		SharedPtr<TextureView> textureView = CreateTextureViewCore(desc);
		mDeviceObjects.push_back(textureView);
		return textureView;
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		SharedPtr<Sampler> sampler = CreateSamplerCore(desc);
		mDeviceObjects.push_back(sampler);
		return sampler;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		SharedPtr<Pipeline> pipeline = CreateGraphicsPipelineCore(desc);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateComputePipeline(const ComputePipelineDesc& desc)
	{
		SharedPtr<Pipeline> pipeline = CreateComputePipelineCore(desc);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	SharedPtr<GraphicsMemory> GraphicsDevice::CreateGraphicsMemory(const GraphicsMemoryDesc& desc)
	{
		SharedPtr<GraphicsMemory> memory = CreateGraphicsMemoryCore(desc);
		mDeviceObjects.push_back(memory);
		return memory;
	}

	SharedPtr<CommandBuffer> GraphicsDevice::CreateCommandBuffer(const CommandBufferDesc& desc)
	{
		SharedPtr<CommandBuffer> commandView = CreateCommandBufferCore(desc);
		mDeviceObjects.push_back(commandView);
		return commandView;
	}

	SharedPtr<CommandPool> GraphicsDevice::CreateCommandPool(const CommandPoolDesc& desc)
	{
		SharedPtr<CommandPool> commandPool = CreateCommandPoolCore(desc);
		mDeviceObjects.push_back(commandPool);
		return commandPool;
	}

	SharedPtr<DescriptorSet> GraphicsDevice::CreateDescriptorSet(const DescriptorSetDesc& desc)
	{
		SharedPtr<DescriptorSet> descriptorSet = CreateDescriptorSetCore(desc);
		mDeviceObjects.push_back(descriptorSet);
		return descriptorSet;
	}

	SharedPtr<DescriptorPool> GraphicsDevice::CreateDescriptorPool(const DescriptorPoolDesc& desc)
	{
		SharedPtr<DescriptorPool> descriptorPool = CreateDescriptorPoolCore(desc);
		mDeviceObjects.push_back(descriptorPool);
		return descriptorPool;
	}

	SharedPtr<DescriptorLayout> GraphicsDevice::CreateDescriptorLayout(const DescriptorLayoutDesc& desc)
	{
		SharedPtr<DescriptorLayout> descriptorLayout = CreateDescriptorLayoutCore(desc);
		mDeviceObjects.push_back(descriptorLayout);
		return descriptorLayout;
	}

	SharedPtr<Fence> GraphicsDevice::CreateFence(const FenceDesc& desc)
	{
		SharedPtr<Fence> fence = CreateFenceCore(desc);
		mDeviceObjects.push_back(fence);
		return fence;
	}

	SharedPtr<Semaphore> GraphicsDevice::CreateSyncSemaphore(const SemaphoreDesc& desc)
	{
		SharedPtr<Semaphore> semaphore = CreateSyncSemaphoreCore(desc);
		mDeviceObjects.push_back(semaphore);
		return semaphore;
	}

	SharedPtr<RenderPass> GraphicsDevice::CreateRenderPass(const RenderPassDesc& desc)
	{
		SharedPtr<RenderPass> renderPass = CreateRenderPassCore(desc);
		mDeviceObjects.push_back(renderPass);
		return renderPass;
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::BorrowGraphicsQueue(const GraphicsQueueDesc& desc)
	{
		SharedPtr<GraphicsQueue> queue = BorrowGraphicsQueueCore(desc);
		mDeviceObjects.push_back(queue);
		return queue;
	}

	void GraphicsDevice::ResetFences(Fence** ppFences, byte amount)
	{
		ResetFencesCore(ppFences, amount);
	}

	void GraphicsDevice::WaitForFences(Fence** ppFences, byte amount)
	{
		WaitForFencesCore(ppFences, amount);
	}

	void GraphicsDevice::WaitForIdleDevice()
	{
		WaitForIdleDeviceCore();
	}

	void GraphicsDevice::WaitQueueDefault(const GraphicsQueueType type)
	{
		WaitQueueDefaultCore(type);
	}

	void GraphicsDevice::UpdateCPUBuffer(GraphicsBuffer* buffer, const GraphicsBufferUpdateDesc& desc)
	{
		UpdateCPUBufferCore(buffer, desc);
	}

	void GraphicsDevice::UpdateDescriptorSet(DescriptorSet* descriptorSet, const DescriptorSetUpdateDesc& desc)
	{
		UpdateDescriptorSetCore(descriptorSet, desc);
	}

	void GraphicsDevice::CopyDescriptorSet(DescriptorSet* pSrcDescriptorSet, DescriptorSet* pDstDescriptorSet, const DescriptorSetCopyDesc& desc)
	{
		CopyDescriptorSetCore(pSrcDescriptorSet, pDstDescriptorSet, desc);
	}

	void GraphicsDevice::SubmitCommandBuffers(CommandBuffer** ppCmdLists, const byte cmdListCount, const GraphicsQueue* pTargetQueue,
		Semaphore** ppSignalSemaphores, const uint32 signalSemaphoreCount, Semaphore** ppWaitSemaphores, const PipelineStageFlags* pWaitStageFlags,
		const uint32 waitSemaphoreCount, const Fence* pSignalFence)
	{
		SubmitCommandBuffersCore(ppCmdLists, cmdListCount, pTargetQueue, ppSignalSemaphores, signalSemaphoreCount, ppWaitSemaphores, pWaitStageFlags, waitSemaphoreCount, pSignalFence);
	}
}
