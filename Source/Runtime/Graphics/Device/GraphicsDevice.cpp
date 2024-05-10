#include "GraphicsDevice.h"

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureView.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/RenderPass/RenderPass.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Sampler/Sampler.h>

#include <Runtime/Window/WindowManager.h>
#include <Runtime/Graphics/GraphicsManager.h>

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : mGraphicsAPI(desc.API), mGraphicsInstance(desc.Instance), mSwapchain(nullptr)
	{
		// Check for the best adapter
		GraphicsAdapter tempAdapter = {};
		for (auto& adapter : desc.Instance->GetAdapters())
		{
			tempAdapter = adapter.AdapterScore > tempAdapter.AdapterScore ? adapter : tempAdapter;
		}

		mGraphicsAdapter = tempAdapter;
		CORE_LOG(HE_VERBOSE, "GraphicsDevice", "Chosen Adapter: %s", mGraphicsAdapter.ProductName.c_str());

		GraphicsManager::GetInstanceAPI().SetGraphicsDevice(this);
	}

	sharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		if (mSwapchain == nullptr)
			mSwapchain = CreateSwapchainImpl(desc);

		WindowManager::GetInstanceAPI().GetDefaultWindow()->SetConnectedSwapchain(mSwapchain);

		return mSwapchain;
	}

	sharedPtr<GraphicsQueue> GraphicsDevice::CreateGraphicsQueue(const GraphicsQueueDesc& desc)
	{
		auto queue = CreateGraphicsQueueImpl(desc);
		mDeviceObjects.push_back(queue);
		return queue;
	}

	sharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		auto texture = CreateTextureImpl(desc);
		mDeviceObjects.push_back(texture);
		return texture;
	}

	sharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		auto textureView = CreateTextureViewImpl(desc);
		mDeviceObjects.push_back(textureView);
		return textureView;
	}

	sharedPtr<Semaphore> GraphicsDevice::CreateSyncSemaphore()
	{
		auto semaphore = CreateSyncSemaphoreImpl();
		mDeviceObjects.push_back(semaphore);
		return semaphore;
	}

	sharedPtr<Fence> GraphicsDevice::CreateFence(const FenceDesc& desc)
	{
		auto fence = CreateFenceImpl(desc);
		mDeviceObjects.push_back(fence);
		return fence;
	}

	sharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		auto shader = CreateShaderImpl(desc);
		mDeviceObjects.push_back(shader);
		return shader;
	}

	sharedPtr<RenderPass> GraphicsDevice::CreateRenderPass(const RenderPassDesc& desc)
	{
		auto renderPass = CreateRenderPassImpl(desc);
		mDeviceObjects.push_back(renderPass);
		return renderPass;
	}

	sharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		auto pipeline = CreateGraphicsPipelineImpl(desc);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	sharedPtr<CommandBuffer> GraphicsDevice::CreateCommandBuffer(const CommandBufferDesc& desc)
	{
		auto commandBuffer = CreateCommandBufferImpl(desc);
		mDeviceObjects.push_back(commandBuffer);
		return commandBuffer;
	}

	sharedPtr<CommandPool> GraphicsDevice::CreateCommandPool(const CommandPoolDesc& desc)
	{
		auto commandPool = CreateCommandPoolImpl(desc);
		mDeviceObjects.push_back(commandPool);
		return commandPool;
	}

	sharedPtr<GraphicsMemory> GraphicsDevice::CreateGraphicsMemory(const GraphicsMemoryDesc& desc)
	{
		auto memory = CreateGraphicsMemoryImpl(desc);
		mDeviceObjects.push_back(memory);
		return memory;
	}

	sharedPtr<GraphicsBuffer> GraphicsDevice::CreateGraphicsBuffer(const GraphicsBufferDesc& desc)
	{
		auto buffer = CreateGraphicsBufferImpl(desc);
		mDeviceObjects.push_back(buffer);
		return buffer;
	}

	sharedPtr<DescriptorSet> GraphicsDevice::CreateDescriptorSet(const DescriptorSetDesc& desc)
	{
		auto descriptorSet = CreateDescriptorSetImpl(desc);
		mDeviceObjects.push_back(descriptorSet);
		return descriptorSet;
	}

	sharedPtr<DescriptorPool> GraphicsDevice::CreateDescriptorPool(const DescriptorPoolDesc& desc)
	{
		auto descriptorPool = CreateDescriptorPoolImpl(desc);
		mDeviceObjects.push_back(descriptorPool);
		return descriptorPool;
	}

	sharedPtr<DescriptorLayout> GraphicsDevice::CreateDescriptorLayout(const DescriptorLayoutDesc& desc)
	{
		auto descriptorLayout = CreateDescriptorLayoutImpl(desc);
		mDeviceObjects.push_back(descriptorLayout);
		return descriptorLayout;
	}

	sharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		auto sampler = CreateSamplerImpl(desc);
		mDeviceObjects.push_back(sampler);
		return sampler;
	}

	void GraphicsDevice::WaitForSemaphore(Semaphore** ppSemaphores, uint32 amount)
	{
		WaitForSemaphoreImpl(ppSemaphores, amount);
	}

	void GraphicsDevice::WaitForFence(Fence** ppFences, uint32 amount)
	{
		WaitForFenceImpl(ppFences, amount);
	}

	void GraphicsDevice::ResetFences(Fence** ppFences, uint32 amount)
	{
		ResetFencesImpl(ppFences, amount);
	}

	void GraphicsDevice::UpdateBufferData(GraphicsBuffer* pBuffer, BufferDataUpdateDesc& desc)
	{
		UpdateBufferDataImpl(pBuffer, desc);
	}

	void GraphicsDevice::WaitForIdle()
	{
		WaitForIdleImpl();
	}

	void GraphicsDevice::WaitQueueIdle(GraphicsQueue* pQueue)
	{
		WaitQueueIdleImpl(pQueue);
	}

	void GraphicsDevice::SubmitToQueue(GraphicsQueue* pQueue, CommandBuffer** ppCommandBuffers, uint32 amount, Semaphore** ppWaitSemaphores,
		uint32 waitSemaphoreCount, PipelineStageFlags* stageFlags, Semaphore** ppSignalSemaphores, uint32 signalSemaphoreCount, Fence* pFence)
	{
		SubmitToQueueImpl(pQueue, ppCommandBuffers, amount, ppWaitSemaphores, waitSemaphoreCount, stageFlags, ppSignalSemaphores, signalSemaphoreCount, pFence);
	}

	void GraphicsDevice::CopyDescriptorSet(DescriptorSet* pSrcSet, DescriptorSet* pDstSet, DescriptorSetCopyDesc& desc)
	{
		CopyDescriptorSetImpl(pSrcSet, pDstSet, desc);
	}

	void GraphicsDevice::UpdateDescriptorSet(DescriptorSet* pDstSet, DescriptorSetUpdateDesc& desc)
	{
		UpdateDescriptorSetImpl(pDstSet, desc);
	}
}
