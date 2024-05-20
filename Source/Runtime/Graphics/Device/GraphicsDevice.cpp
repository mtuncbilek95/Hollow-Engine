#include "GraphicsDevice.h"

#include <Runtime/Window/WindowManager.h>

#include <Runtime/Graphics/Swapchain/Swapchain.h>
#include <Runtime/Graphics/Queue/GraphicsQueue.h>
#include <Runtime/Graphics/Texture/Texture.h>
#include <Runtime/Graphics/Texture/TextureBuffer.h>
#include <Runtime/Graphics/Semaphore/Semaphore.h>
#include <Runtime/Graphics/Fence/Fence.h>
#include <Runtime/Graphics/Shader/Shader.h>
#include <Runtime/Graphics/Pipeline/Pipeline.h>
#include <Runtime/Graphics/Command/CommandBuffer.h>
#include <Runtime/Graphics/Command/CommandPool.h>
#include <Runtime/Graphics/Memory/GraphicsMemory.h>
#include <Runtime/Graphics/Buffer/GraphicsBuffer.h>
#include <Runtime/Graphics/Descriptor/DescriptorSet.h>
#include <Runtime/Graphics/Descriptor/DescriptorPool.h>
#include <Runtime/Graphics/Descriptor/DescriptorLayout.h>
#include <Runtime/Graphics/Sampler/Sampler.h>

namespace Hollow
{
	GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : Object(), mInstance(desc.Instance), mGraphicsQueueCount(desc.GraphicsQueueCount), 
		mComputeQueueCount(desc.ComputeQueueCount), mTransferQueueCount(desc.TransferQueueCount), mOwnedSwapchain(nullptr), mDeviceObjects(), mAdapter()
	{
		mInstance->EnumerateAdapters();

		SharedPtr<GraphicsAdapter> tempAdapter = std::make_shared<GraphicsAdapter>();

		for (auto& adapter : mInstance->GetAdapters())
		{
			tempAdapter = adapter->AdapterScore > tempAdapter->AdapterScore ? adapter : tempAdapter;
		}

		mAdapter = tempAdapter;

		CORE_LOG(HE_INFO, "Graphics Device", "%s has been selected as the primary adapter", mAdapter->ProductName.c_str());
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		if (mOwnedSwapchain)
		{
			CORE_LOG(HE_WARNING, "Graphics Device", "Swapchain already exists");
			return mOwnedSwapchain;
		}

		mOwnedSwapchain = CreateSwapchainImpl(desc);
		mDeviceObjects.push_back(mOwnedSwapchain);
		WindowManager::GetInstanceAPI().GetDefaultWindow()->SetConnectedSwapchain(mOwnedSwapchain);
		return mOwnedSwapchain;
	}

	SharedPtr<Texture> GraphicsDevice::CreateTexture(const TextureDesc& desc)
	{
		auto texture = CreateTextureImpl(desc);
		mDeviceObjects.push_back(texture);
		return texture;
	}

	SharedPtr<TextureBuffer> GraphicsDevice::CreateTextureBuffer(const TextureBufferDesc& desc)
	{
		auto textureBuffer = CreateTextureBufferImpl(desc);
		mDeviceObjects.push_back(textureBuffer);
		return textureBuffer;
	}

	SharedPtr<GraphicsMemory> GraphicsDevice::CreateGraphicsMemory(const GraphicsMemoryDesc& desc)
	{
		auto memory = CreateGraphicsMemoryImpl(desc);
		mDeviceObjects.push_back(memory);
		return memory;
	}

	SharedPtr<Semaphore> GraphicsDevice::CreateSyncSemaphore()
	{
		auto semaphore = CreateSyncSemaphoreImpl();
		mDeviceObjects.push_back(semaphore);
		return semaphore;
	}

	SharedPtr<Fence> GraphicsDevice::CreateSyncFence(const FenceDesc& desc)
	{
		auto fence = CreateSyncFenceImpl(desc);
		mDeviceObjects.push_back(fence);
		return fence;
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
	{
		auto queue = CreateQueueImpl(desc);
		mDeviceObjects.push_back(queue);
		return queue;
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		auto sampler = CreateSamplerImpl(desc);
		mDeviceObjects.push_back(sampler);
		return sampler;
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		auto shader = CreateShaderImpl(desc);
		mDeviceObjects.push_back(shader);
		return shader;
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		auto pipeline = CreateGraphicsPipelineImpl(desc);
		mDeviceObjects.push_back(pipeline);
		return pipeline;
	}

	SharedPtr<CommandBuffer> GraphicsDevice::CreateCommandBuffer(const CommandBufferDesc& desc)
	{
		auto commandBuffer = CreateCommandBufferImpl(desc);
		mDeviceObjects.push_back(commandBuffer);
		return commandBuffer;
	}

	SharedPtr<CommandPool> GraphicsDevice::CreateCommandPool(const CommandPoolDesc& desc)
	{
		auto commandPool = CreateCommandPoolImpl(desc);
		mDeviceObjects.push_back(commandPool);
		return commandPool;
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateGraphicsBuffer(const GraphicsBufferDesc& desc)
	{
		auto buffer = CreateGraphicsBufferImpl(desc);
		mDeviceObjects.push_back(buffer);
		return buffer;
	}

	SharedPtr<DescriptorSet> GraphicsDevice::CreateDescriptorSet(const DescriptorSetDesc& desc)
	{
		auto descriptorSet = CreateDescriptorSetImpl(desc);
		mDeviceObjects.push_back(descriptorSet);
		return descriptorSet;
	}

	SharedPtr<DescriptorPool> GraphicsDevice::CreateDescriptorPool(const DescriptorPoolDesc& desc)
	{
		auto descriptorPool = CreateDescriptorPoolImpl(desc);
		mDeviceObjects.push_back(descriptorPool);
		return descriptorPool;
	}

	SharedPtr<DescriptorLayout> GraphicsDevice::CreateDescriptorLayout(const DescriptorLayoutDesc& desc)
	{
		auto descriptorLayout = CreateDescriptorLayoutImpl(desc);
		mDeviceObjects.push_back(descriptorLayout);
		return descriptorLayout;
	}

	void GraphicsDevice::WaitForSemaphore(SharedPtr<Semaphore> ppSemaphores[], uint32 amount)
	{
		WaitForSemaphoreImpl(ppSemaphores, amount);
	}

	void GraphicsDevice::WaitForFence(SharedPtr<Fence> ppFences[], uint32 amount)
	{
		WaitForFenceImpl(ppFences, amount);
	}

	void GraphicsDevice::ResetFences(SharedPtr<Fence> ppFences[], uint32 amount)
	{
		ResetFencesImpl(ppFences, amount);
	}

	void GraphicsDevice::UpdateBufferData(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc)
	{
		UpdateBufferDataImpl(pBuffer, desc);
	}

	void GraphicsDevice::WaitForIdle()
	{
		WaitForIdleImpl();
	}

	void GraphicsDevice::WaitQueueIdle(SharedPtr<GraphicsQueue> pQueue)
	{
		WaitQueueIdleImpl(pQueue);
	}

	void GraphicsDevice::SubmitToQueue(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], uint32 amount, 
		SharedPtr<Semaphore> ppWaitSemaphores[], uint32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], 
		uint32 signalSemaphoreCount, SharedPtr<Fence> pFence)
	{
		SubmitToQueueImpl(pQueue, ppCommandBuffers, amount, ppWaitSemaphores, waitSemaphoreCount, stageFlags, ppSignalSemaphores, signalSemaphoreCount, pFence);
	}

	void GraphicsDevice::CopyDescriptorSet(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc)
	{
		CopyDescriptorSetImpl(pSrcSet, pDstSet, desc);
	}

	void GraphicsDevice::UpdateDescriptorSet(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc)
	{
		UpdateDescriptorSetImpl(pDstSet, desc);
	}
}
