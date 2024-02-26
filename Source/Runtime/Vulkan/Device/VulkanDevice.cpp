#include "VulkanDevice.h"

#include <Runtime/Vulkan/Adapter/VulkanAdapter.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Texture/VulkanTextureView.h>
#include <Runtime/Vulkan/Shader/VulkanShader.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>
#include <Runtime/Vulkan/RenderPass/VulkanRenderPass.h>
#include <Runtime/Vulkan/Sampler/VulkanSampler.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>
#include <Runtime/Vulkan/Buffer/VulkanGraphicsBuffer.h>
#include <Runtime/Vulkan/Command/VulkanCommandBuffer.h>
#include <Runtime/Vulkan/Command/VulkanCommandPool.h>

namespace Hollow
{
	PFN_vkCmdBeginRenderingKHR VulkanDevice::vkCmdBeginRenderingKHR = NULL;
	PFN_vkCmdEndRenderingKHR VulkanDevice::vkCmdEndRenderingKHR = NULL;

	VulkanDevice::VulkanDevice(const GraphicsDeviceDesc& desc) : GraphicsDevice(desc), mVkLogicalDevice(VK_NULL_HANDLE)
	{
		mVkPhysicalDevice = std::dynamic_pointer_cast<VulkanAdapter>(desc.Adapter)->GetVkPhysicalDevice();

		VulkanDeviceDesc pDesc = (VulkanDeviceDesc&)desc;

		// Get the queue family count
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, nullptr);
		DEV_ASSERT(queueFamilyCount > 0, "VulkanDevice", "Failed to get Queue Family Count.");

		// Get the queue family properties
		Array<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, queueFamilyProperties.data());

		// Get the queue family indices
		for (byte i = 0; i < queueFamilyCount; i++)
		{
			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				mGraphicsQueueFamily.FamilyIndex = i;
				mGraphicsQueueFamily.QueueCapacity = queueFamilyProperties[i].queueCount;
				mGraphicsQueueFamily.RequestedCount = pDesc.GraphicsQueueCount > queueFamilyProperties[i].queueCount ? queueFamilyProperties[i].queueCount : pDesc.GraphicsQueueCount;
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			{
				mComputeQueueFamily.FamilyIndex = i;
				mComputeQueueFamily.QueueCapacity = queueFamilyProperties[i].queueCount;
				mComputeQueueFamily.RequestedCount = pDesc.ComputeQueueCount > queueFamilyProperties[i].queueCount ? queueFamilyProperties[i].queueCount : pDesc.ComputeQueueCount;
			}

			if (queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				mTransferQueueFamily.FamilyIndex = i;
				mTransferQueueFamily.QueueCapacity = queueFamilyProperties[i].queueCount;
				mTransferQueueFamily.RequestedCount = pDesc.TransferQueueCount > queueFamilyProperties[i].queueCount ? queueFamilyProperties[i].queueCount : pDesc.TransferQueueCount;
			}
		}

		// Check if the queue families are valid
		DEV_ASSERT(mGraphicsQueueFamily.QueueCapacity > 0, "VulkanDevice", "There is no queue support for graphics.");
		DEV_ASSERT(mComputeQueueFamily.QueueCapacity > 0, "VulkanDevice", "There is no queue support for compute.");
		DEV_ASSERT(mTransferQueueFamily.QueueCapacity > 0, "VulkanDevice", "There is no queue support for transfer.");

		// Fill the queues
		float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		Array<VkDeviceQueueCreateInfo> queueCreateInfos;

		// Add graphics queue create infos
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
		graphicsQueueCreateInfo.queueCount = mGraphicsQueueFamily.QueueCapacity;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;
		graphicsQueueCreateInfo.pNext = nullptr;

		queueCreateInfos.push_back(graphicsQueueCreateInfo);

		// Add compute queue create infos
		VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
		computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueCreateInfo.queueFamilyIndex = mComputeQueueFamily.FamilyIndex;
		computeQueueCreateInfo.queueCount = mComputeQueueFamily.QueueCapacity;
		computeQueueCreateInfo.pQueuePriorities = queuePriorities;
		computeQueueCreateInfo.pNext = nullptr;

		queueCreateInfos.push_back(computeQueueCreateInfo);

		// Add transfer queue create infos
		VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.queueFamilyIndex = mTransferQueueFamily.FamilyIndex;
		transferQueueCreateInfo.queueCount = mTransferQueueFamily.QueueCapacity;
		transferQueueCreateInfo.pQueuePriorities = queuePriorities;
		transferQueueCreateInfo.pNext = nullptr;

		queueCreateInfos.push_back(transferQueueCreateInfo);

		// Add logical device extensions
		Array<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_MULTIVIEW_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_MAINTENANCE2_EXTENSION_NAME);


		// Add more extensions
		for (const char* extension : ((VulkanDeviceDesc&)desc).deviceExtensions)
		{
			if (std::find(deviceExtensions.begin(), deviceExtensions.end(), extension) == deviceExtensions.end())
				deviceExtensions.push_back(extension);
		}

		// Add logical device features
		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures = ((VulkanDeviceDesc&)desc).deviceFeatures.features;

		// Create Device Info
		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.flags = VkDeviceCreateFlags();
		deviceCreateInfo.pNext = nullptr;

		DEV_ASSERT(vkCreateDevice(mVkPhysicalDevice, &deviceCreateInfo, nullptr, &mVkLogicalDevice) == VK_SUCCESS, "VulkanDevice",
			"Failed to create Logical Device.");

		CORE_LOG(HE_VERBOSE, "VulkanDevice", "Created Logical Device.");

		// Reserve the queues
		mGraphicsQueueFamily.FreeQueues.reserve(desc.GraphicsQueueCount);
		mComputeQueueFamily.FreeQueues.reserve(desc.ComputeQueueCount);
		mTransferQueueFamily.FreeQueues.reserve(desc.TransferQueueCount);

		// Get graphics queues
		for (byte i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue pQueue;
			vkGetDeviceQueue(mVkLogicalDevice, mGraphicsQueueFamily.FamilyIndex, i, &pQueue);
			mGraphicsQueueFamily.FreeQueues.push_back(pQueue);
		}

		// Get compute queues
		for (byte i = 0; i < mComputeQueueFamily.RequestedCount; i++)
		{
			VkQueue pQueue;
			vkGetDeviceQueue(mVkLogicalDevice, mComputeQueueFamily.FamilyIndex, i, &pQueue);
			mComputeQueueFamily.FreeQueues.push_back(pQueue);
		}

		// Get transfer queues
		for (byte i = 0; i < mTransferQueueFamily.RequestedCount; i++)
		{
			VkQueue pQueue;
			vkGetDeviceQueue(mVkLogicalDevice, mTransferQueueFamily.FamilyIndex, i, &pQueue);
			mTransferQueueFamily.FreeQueues.push_back(pQueue);
		}

		CORE_LOG(HE_VERBOSE, "VulkanDevice", "Reserved Queues.");
	}

	int32 VulkanDevice::CatchQueueFamilyIndex(const GraphicsQueueType type)
	{
		switch (type)
		{
		case GraphicsQueueType::Graphics:
			return mGraphicsQueueFamily.FamilyIndex;
		case GraphicsQueueType::Compute:
			return mComputeQueueFamily.FamilyIndex;
		case GraphicsQueueType::Transfer:
			return mTransferQueueFamily.FamilyIndex;
		default:
			return -1;
		}
	}

	VkQueue VulkanDevice::CatchGraphicsQueue(const GraphicsQueueType type)
	{
		switch (type)
		{
		case GraphicsQueueType::Graphics:
			return mGraphicsQueueFamily.GetAvailableQueue();
		case GraphicsQueueType::Compute:
			return mComputeQueueFamily.GetAvailableQueue();
		case GraphicsQueueType::Transfer:
			return mTransferQueueFamily.GetAvailableQueue();
		default:
			return VK_NULL_HANDLE;
		}
	}

	SharedPtr<VulkanTexture> VulkanDevice::CreateVkTextureForSwapchain(const TextureDesc& desc, VkImage image)
	{
		return std::make_shared<VulkanTexture>(desc, image, this);
	}

	SharedPtr<VulkanTextureView> VulkanDevice::CreateVkTextureViewForSwapchain(const TextureViewDesc& desc, VkImageView imageView)
	{
		return std::make_shared<VulkanTextureView>(desc, imageView, this);
	}

	void VulkanDevice::OnShutdown()
	{
		vkDestroyDevice(mVkLogicalDevice, nullptr);
		CORE_LOG(HE_WARNING, "VulkanDevice", "Shutting down...");
	}

	SharedPtr<Swapchain> VulkanDevice::CreateSwapchainCore(const SwapchainDesc& desc)
	{
		return std::make_shared<VulkanSwapchain>(desc, this);
	}

	SharedPtr<Shader> VulkanDevice::CreateShaderCore(const ShaderDesc& desc)
	{
		return std::make_shared<VulkanShader>(desc, this);
	}

	SharedPtr<GraphicsBuffer> VulkanDevice::CreateGraphicsBufferCore(const GraphicsBufferDesc& desc)
	{
		return std::make_shared<VulkanGraphicsBuffer>(desc, this);
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureCore(const TextureDesc& desc)
	{
		return std::make_shared<VulkanTexture>(desc, this);
	}

	SharedPtr<TextureView> VulkanDevice::CreateTextureViewCore(const TextureViewDesc& desc)
	{
		return std::make_shared<VulkanTextureView>(desc, this);
	}

	SharedPtr<Sampler> VulkanDevice::CreateSamplerCore(const SamplerDesc& desc)
	{
		return std::make_shared<VulkanSampler>(desc, this);
	}

	SharedPtr<Pipeline> VulkanDevice::CreateGraphicsPipelineCore(const GraphicsPipelineDesc& desc)
	{
		return std::make_shared<VulkanPipeline>(desc, this);
	}

	SharedPtr<Pipeline> VulkanDevice::CreateComputePipelineCore(const ComputePipelineDesc& desc)
	{
		return std::make_shared<VulkanPipeline>(desc, this);
	}

	SharedPtr<GraphicsMemory> VulkanDevice::CreateGraphicsMemoryCore(const GraphicsMemoryDesc& desc)
	{
		return std::make_shared<VulkanMemory>(desc, this);
	}

	SharedPtr<RenderPass> VulkanDevice::CreateRenderPassCore(const RenderPassDesc& desc)
	{
		return std::make_shared<VulkanRenderPass>(desc, this);
	}

	SharedPtr<CommandBuffer> VulkanDevice::CreateCommandBufferCore(const CommandBufferDesc& desc)
	{
		return std::make_shared<VulkanCommandBuffer>(desc, this);
	}

	SharedPtr<CommandPool> VulkanDevice::CreateCommandPoolCore(const CommandPoolDesc& desc)
	{
		return std::make_shared<VulkanCommandPool>(desc, this);
	}

	SharedPtr<DescriptorSet> VulkanDevice::CreateDescriptorSetCore(const DescriptorSetDesc& desc)
	{
		return std::make_shared<VulkanDescriptorSet>(desc, this);
	}

	SharedPtr<DescriptorPool> VulkanDevice::CreateDescriptorPoolCore(const DescriptorPoolDesc& desc)
	{
		return std::make_shared<VulkanDescriptorPool>(desc, this);
	}

	SharedPtr<DescriptorLayout> VulkanDevice::CreateDescriptorLayoutCore(const DescriptorLayoutDesc& desc)
	{
		return std::make_shared<VulkanDescriptorLayout>(desc, this);
	}

	SharedPtr<Fence> VulkanDevice::CreateFenceCore(const FenceDesc& desc)
	{
		return std::make_shared<VulkanFence>(desc, this);
	}

	SharedPtr<Semaphore> VulkanDevice::CreateSyncSemaphoreCore(const SemaphoreDesc& desc)
	{
		return std::make_shared<VulkanSemaphore>(desc, this);
	}

	SharedPtr<GraphicsQueue> VulkanDevice::BorrowGraphicsQueueCore(const GraphicsQueueDesc& desc)
	{
		return std::make_shared<VulkanQueue>(desc, this);
	}

	void VulkanDevice::ResetFencesCore(Fence** ppFences, byte amount)
	{
		VkFence* fences = new VkFence[amount];

		for (byte i = 0; i < amount; i++)
		{
			fences[i] = ((VulkanFence*)ppFences[i])->GetVkFence();
		}

		vkResetFences(mVkLogicalDevice, amount, fences);
	}

	void VulkanDevice::WaitForFencesCore(Fence** ppFences, byte amount)
	{
		VkFence* fences = new VkFence[amount];

		for (byte i = 0; i < amount; i++)
		{
			fences[i] = ((VulkanFence*)ppFences[i])->GetVkFence();
		}

		vkWaitForFences(mVkLogicalDevice, amount, fences, VK_TRUE, UINT64_MAX);
	}

	void VulkanDevice::WaitForIdleDeviceCore()
	{
		DEV_ASSERT(vkDeviceWaitIdle(mVkLogicalDevice) == VK_SUCCESS, "VulkanDevice", "Failed to wait for device to be idle.");
	}

	void VulkanDevice::WaitQueueDefaultCore(const GraphicsQueueType type)
	{
		VkQueue queue = CatchGraphicsQueue(type);
		DEV_ASSERT(vkQueueWaitIdle(queue) == VK_SUCCESS, "VulkanDevice", "Failed to wait for queue to be idle.");
	}

	void VulkanDevice::UpdateCPUBufferCore(GraphicsBuffer** buffer, const GraphicsBufferUpdateDesc& desc)
	{
	}

	void VulkanDevice::UpdateDescriptorSetCore(DescriptorSet** descriptorSet, const DescriptorSetUpdateDesc& desc)
	{
	}

	void VulkanDevice::SubmitCommandBuffersCore(CommandBuffer** commandBuffers, const byte amount, const GraphicsQueueType type, const Fence* pFence)
	{
	}
}