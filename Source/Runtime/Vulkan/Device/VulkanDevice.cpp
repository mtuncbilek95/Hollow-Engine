#include "VulkanDevice.h"

#include <Runtime/Vulkan/Instance/VulkanInstance.h>
#include <Runtime/Vulkan/Swapchain/VulkanSwapchain.h>
#include <Runtime/Vulkan/Semaphore/VulkanSemaphore.h>
#include <Runtime/Vulkan/Texture/VulkanTexture.h>
#include <Runtime/Vulkan/Texture/VulkanTextureBuffer.h>
#include <Runtime/Vulkan/Fence/VulkanFence.h>
#include <Runtime/Vulkan/Queue/VulkanQueue.h>
#include <Runtime/Vulkan/Buffer/VulkanBuffer.h>
#include <Runtime/Vulkan/Memory/VulkanMemory.h>
#include <Runtime/Vulkan/Shader/VulkanShader.h>
#include <Runtime/Vulkan/Command/VulkanCommandBuffer.h>
#include <Runtime/Vulkan/Command/VulkanCommandPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorSet.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorPool.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorLayout.h>
#include <Runtime/Vulkan/Sampler/VulkanSampler.h>
#include <Runtime/Vulkan/Pipeline/VulkanPipeline.h>

#include <Runtime/Vulkan/Pipeline/VulkanPipelineUtils.h>
#include <Runtime/Vulkan/Descriptor/VulkanDescriptorUtils.h>

struct Vertex
{
	Vector3f Position;
	Vector3f Color;
	Vector2f TexCoord;
};

namespace Hollow
{
	VulkanDevice::VulkanDevice(const GraphicsDeviceDesc& desc) : GraphicsDevice(desc), mVkDevice(VK_NULL_HANDLE), mVkPhysicalDevice(VK_NULL_HANDLE), mVkInstance(VK_NULL_HANDLE)
	{
		mVkInstance = std::static_pointer_cast<VulkanInstance>(desc.Instance)->GetVkInstance();

		// Get the physical device count
		uint32 deviceCount = 0;
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr);
		CORE_ASSERT(deviceCount > 0, "VulkanDevice", "No physical devices found");

		// Get the physical devices
		ArrayList<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, devices.data());

		// Find the related physical device according to the chosen device
		for (auto& device : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			String deviceName = deviceProperties.deviceName;

			if (deviceName == mAdapter->ProductName)
			{
				mVkPhysicalDevice = device;
				break;
			}
		}

		CORE_ASSERT(mVkPhysicalDevice != VK_NULL_HANDLE, "VulkanDevice", "No physical device found for the given adapter");

		// Get the device properties

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &deviceProperties);

		CORE_LOG(HE_INFO, "VulkanDevice", "Granularity: %d", deviceProperties.limits.bufferImageGranularity);

		// Get the queue family count
		uint32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, nullptr);
		CORE_ASSERT(queueFamilyCount > 0, "VulkanDevice", "No queue families found");

		// Get the queue families
		ArrayList<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mVkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

		// Find the queue families for graphics, compute and transfer
		int index = 0;
		for (auto& prop : queueFamilies)
		{
			if (mGraphicsQueueFamily.FamilyIndex == 255 || mComputeQueueFamily.FamilyIndex == 255 || mTransferQueueFamily.FamilyIndex == 255)
				CORE_LOG(HE_WARNING, "VulkanDevice", "Queue Family Index: %d", index);

			if (prop.queueFlags & VK_QUEUE_GRAPHICS_BIT && mGraphicsQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Graphics Found. Count: %d", prop.queueCount);
				mGraphicsQueueFamily.FamilyIndex = index;
				mGraphicsQueueFamily.QueueCapacity = prop.queueCount;
				mGraphicsQueueFamily.RequestedCount = desc.GraphicsQueueCount > prop.queueCount ? prop.queueCount : desc.GraphicsQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_COMPUTE_BIT && mComputeQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Compute Found. Count: %d", prop.queueCount);
				mComputeQueueFamily.FamilyIndex = index;
				mComputeQueueFamily.QueueCapacity = prop.queueCount;
				mComputeQueueFamily.RequestedCount = desc.ComputeQueueCount > prop.queueCount ? prop.queueCount : desc.ComputeQueueCount;
			}
			else if (prop.queueFlags & VK_QUEUE_TRANSFER_BIT && mTransferQueueFamily.FamilyIndex == 255)
			{
				CORE_LOG(HE_INFO, "VulkanDevice", "Queue Transfer Found. Count: %d", prop.queueCount);
				mTransferQueueFamily.FamilyIndex = index;
				mTransferQueueFamily.QueueCapacity = prop.queueCount;
				mTransferQueueFamily.RequestedCount = desc.TransferQueueCount > prop.queueCount ? prop.queueCount : desc.TransferQueueCount;
			}
			index++;
		}

		// Queue priorities
		float queuePriorities[] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
		ArrayList<VkDeviceQueueCreateInfo> queueCreateInfos = {};

		// Graphics Queue Create Info
		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = mGraphicsQueueFamily.FamilyIndex;
		graphicsQueueCreateInfo.queueCount = mGraphicsQueueFamily.QueueCapacity;
		graphicsQueueCreateInfo.pQueuePriorities = queuePriorities;

		queueCreateInfos.push_back(graphicsQueueCreateInfo);

		// Compute Queue Create Info
		VkDeviceQueueCreateInfo computeQueueCreateInfo = {};
		computeQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		computeQueueCreateInfo.queueFamilyIndex = mComputeQueueFamily.FamilyIndex;
		computeQueueCreateInfo.queueCount = mComputeQueueFamily.QueueCapacity;
		computeQueueCreateInfo.pQueuePriorities = queuePriorities;

		queueCreateInfos.push_back(computeQueueCreateInfo);

		// Transfer Queue Create Info
		VkDeviceQueueCreateInfo transferQueueCreateInfo = {};
		transferQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		transferQueueCreateInfo.queueFamilyIndex = mTransferQueueFamily.FamilyIndex;
		transferQueueCreateInfo.queueCount = mTransferQueueFamily.QueueCapacity;
		transferQueueCreateInfo.pQueuePriorities = queuePriorities;

		queueCreateInfos.push_back(transferQueueCreateInfo);

		// Device Extensions
		ArrayList<const char*> deviceExtensions;
		deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);

		//Check if the device supports the extensions
		uint32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(mVkPhysicalDevice, nullptr, &extensionCount, nullptr);
		ArrayList<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(mVkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

#if defined(HOLLOW_DEBUG)
		for(auto& extension : availableExtensions)
			CORE_LOG(HE_INFO, "VulkanDevice", "Available Extension: %s", extension.extensionName);
#endif

		// Device Features
		VkPhysicalDeviceFeatures deviceFeatures = {};
		vkGetPhysicalDeviceFeatures(mVkPhysicalDevice, &deviceFeatures);

		// Dynamic Rendering Features
		VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeatures = {};
		dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR;
		dynamicRenderingFeatures.pNext = nullptr;
		dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

		// Device Info
		VkDeviceCreateInfo deviceInfo = {};
		deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount = static_cast<uint32>(queueCreateInfos.size());
		deviceInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceInfo.enabledExtensionCount = static_cast<uint32>(deviceExtensions.size());
		deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceInfo.pEnabledFeatures = &deviceFeatures;
		deviceInfo.flags = VkDeviceCreateFlags();
		deviceInfo.pNext = &dynamicRenderingFeatures;

		CORE_ASSERT(vkCreateDevice(mVkPhysicalDevice, &deviceInfo, nullptr, &mVkDevice) == VK_SUCCESS, "VulkanDevice", "Failed to create logical device");

		// Reserve the queues for graphics, compute and transfer and store them in the related families.
		mGraphicsQueueFamily.FreeQueues.reserve(mGraphicsQueueFamily.RequestedCount);
		mComputeQueueFamily.FreeQueues.reserve(mComputeQueueFamily.RequestedCount);
		mTransferQueueFamily.FreeQueues.reserve(mTransferQueueFamily.RequestedCount);

		// Get the graphics queues
		for (byte i = 0; i < mGraphicsQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mGraphicsQueueFamily.FamilyIndex, i, &queue);
			mGraphicsQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the compute queues
		for (byte i = 0; i < mComputeQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mComputeQueueFamily.FamilyIndex, i, &queue);
			mComputeQueueFamily.FreeQueues.push_back(queue);
		}

		// Get the transfer queues
		for (byte i = 0; i < mTransferQueueFamily.RequestedCount; i++)
		{
			VkQueue queue;
			vkGetDeviceQueue(mVkDevice, mTransferQueueFamily.FamilyIndex, i, &queue);
			mTransferQueueFamily.FreeQueues.push_back(queue);
		}
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureForSwapchain(const TextureDesc& desc, VkImage image)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTexture>(desc, image, device);
	}

	uint32 VulkanDevice::GetQueueFamilyIndex(GraphicsQueueType type) const
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
			return 255;
		}
	}

	void VulkanDevice::OnShutdown()
	{
		for (int i = static_cast<int>(mDeviceObjects.size()) - 1; i >= 0; --i)
		{
			mDeviceObjects[i]->OnShutdown();
			mDeviceObjects[i].reset(); // Release the shared pointer
		}

		if (mOwnedSwapchain != nullptr)
		{
			//mOwnedSwapchain->OnShutdown();
			mOwnedSwapchain.reset();
		}

		if (mVkDevice != VK_NULL_HANDLE)
			vkDestroyDevice(mVkDevice, nullptr);

		mVkInstance = VK_NULL_HANDLE;
		mVkPhysicalDevice = VK_NULL_HANDLE;

		mGraphicsQueueFamily.FamilyIndex = 255;
		mComputeQueueFamily.FamilyIndex = 255;
		mTransferQueueFamily.FamilyIndex = 255;

		CORE_LOG(HE_INFO, "VulkanDevice", "Device shutdown");
	}

	SharedPtr<Swapchain> VulkanDevice::CreateSwapchainImpl(const SwapchainDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanSwapchain>(desc, device);
	}

	SharedPtr<Texture> VulkanDevice::CreateTextureImpl(const TextureDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTexture>(desc, device);
	}

	SharedPtr<TextureBuffer> VulkanDevice::CreateTextureBufferImpl(const TextureBufferDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanTextureBuffer>(desc, device);
	}

	SharedPtr<GraphicsMemory> VulkanDevice::CreateGraphicsMemoryImpl(const GraphicsMemoryDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanMemory>(desc, device);
	}

	SharedPtr<Semaphore> VulkanDevice::CreateSyncSemaphoreImpl()
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanSemaphore>(device);
	}

	SharedPtr<Fence> VulkanDevice::CreateSyncFenceImpl(const FenceDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanFence>(desc, device);
	}

	SharedPtr<GraphicsQueue> VulkanDevice::CreateQueueImpl(const GraphicsQueueDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		switch (desc.QueueType)
		{
		case GraphicsQueueType::Graphics:
		{
			if (mGraphicsQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mGraphicsQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free graphics queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Compute:
		{
			if (mComputeQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mComputeQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free compute queue found");
				return nullptr;
			}
		}
		case GraphicsQueueType::Transfer:
		{
			if (mTransferQueueFamily.HasFreeQueue())
				return std::make_shared<VulkanQueue>(desc, mTransferQueueFamily.GetFreeQueue(), device);
			else
			{
				CORE_LOG(HE_ERROR, "VulkanDevice", "No free transfer queue found");
				return nullptr;
			}
		}
		default:
			CORE_LOG(HE_ERROR, "VulkanDevice", "Invalid queue type");
			return nullptr;
		}
	}

	SharedPtr<Sampler> VulkanDevice::CreateSamplerImpl(const SamplerDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanSampler>(desc, device);
	}

	SharedPtr<Shader> VulkanDevice::CreateShaderImpl(const ShaderDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanShader>(desc, device);
	}

	SharedPtr<Pipeline> VulkanDevice::CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanPipeline>(desc, device);
	}

	SharedPtr<CommandBuffer> VulkanDevice::CreateCommandBufferImpl(const CommandBufferDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanCommandBuffer>(desc, device);
	}

	SharedPtr<CommandPool> VulkanDevice::CreateCommandPoolImpl(const CommandPoolDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanCommandPool>(desc, device);
	}

	SharedPtr<GraphicsBuffer> VulkanDevice::CreateGraphicsBufferImpl(const GraphicsBufferDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanBuffer>(desc, device);
	}

	SharedPtr<DescriptorSet> VulkanDevice::CreateDescriptorSetImpl(const DescriptorSetDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanDescriptorSet>(desc, device);
	}

	SharedPtr<DescriptorPool> VulkanDevice::CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanDescriptorPool>(desc, device);
	}

	SharedPtr<DescriptorLayout> VulkanDevice::CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc)
	{
		auto device = std::static_pointer_cast<VulkanDevice>(GetSharedPtr());
		return std::make_shared<VulkanDescriptorLayout>(desc, device);
	}

	void VulkanDevice::WaitForSemaphoreImpl(SharedPtr<Semaphore> ppSemaphores[], uint32 amount)
	{
		VkSemaphore semaphores[32];
		for (byte i = 0; i < amount; i++)
			semaphores[i] = std::static_pointer_cast<VulkanSemaphore>(ppSemaphores[i])->GetVkSemaphore();

		uint64 waitValue = 1;

		VkSemaphoreWaitInfo waitInfo = {};
		waitInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
		waitInfo.semaphoreCount = amount;
		waitInfo.pSemaphores = semaphores;
		waitInfo.pValues = &waitValue;
		waitInfo.flags = 0;
		waitInfo.pNext = nullptr;

		CORE_ASSERT(vkWaitSemaphores(mVkDevice, &waitInfo, UINT64_MAX) == VK_SUCCESS, "VulkanDevice", "Failed to wait for semaphores");
	}

	void VulkanDevice::WaitForFenceImpl(SharedPtr<Fence> ppFences[], uint32 amount)
	{
		VkFence fences[32];
		for (byte i = 0; i < amount; i++)
			fences[i] = std::static_pointer_cast<VulkanFence>(ppFences[i])->GetVkFence();

		CORE_ASSERT(vkWaitForFences(mVkDevice, amount, fences, VK_TRUE, UINT64_MAX) == VK_SUCCESS, "VulkanDevice", "Failed to wait for fences");
	}

	void VulkanDevice::ResetFencesImpl(SharedPtr<Fence> ppFences[], uint32 amount)
	{
		VkFence fences[32];
		for (byte i = 0; i < amount; i++)
			fences[i] = std::static_pointer_cast<VulkanFence>(ppFences[i])->GetVkFence();

		CORE_ASSERT(vkResetFences(mVkDevice, amount, fences) == VK_SUCCESS, "VulkanDevice", "Failed to reset fences");
	}

	void VulkanDevice::UpdateBufferDataImpl(SharedPtr<GraphicsBuffer> pBuffer, BufferDataUpdateDesc& desc)
	{
		auto pMemory = std::static_pointer_cast<VulkanMemory>(pBuffer->GetMemory());
		auto pTarget = std::static_pointer_cast<VulkanBuffer>(pBuffer);

		void* pData;

		CORE_ASSERT(vkMapMemory(mVkDevice, pMemory->GetVkDeviceMemory(), pTarget->GetVkAlignedOffset() + desc.OffsetInBytes, desc.Memory.GetSize(), 0, &pData) == VK_SUCCESS,
			"VulkanDevice", "Failed to map memory");
		memcpy(pData, desc.Memory.GetBuffer(), desc.Memory.GetSize());
		vkUnmapMemory(mVkDevice, pMemory->GetVkDeviceMemory());
	}

	void VulkanDevice::WaitForIdleImpl()
	{
		CORE_ASSERT(vkDeviceWaitIdle(mVkDevice) == VK_SUCCESS, "VulkanDevice", "Failed to wait for device idle");
	}

	void VulkanDevice::WaitQueueIdleImpl(SharedPtr<GraphicsQueue> pQueue)
	{
		CORE_ASSERT(vkQueueWaitIdle(std::static_pointer_cast<VulkanQueue>(pQueue)->GetVkQueue()) == VK_SUCCESS, "VulkanDevice", "Failed to wait for queue idle");
	}

	void VulkanDevice::SubmitToQueueImpl(SharedPtr<GraphicsQueue> pQueue, SharedPtr<CommandBuffer> ppCommandBuffers[], uint32 amount, 
		SharedPtr<Semaphore> ppWaitSemaphores[], uint32 waitSemaphoreCount, PipelineStageFlags stageFlags[], SharedPtr<Semaphore> ppSignalSemaphores[], 
		uint32 signalSemaphoreCount, SharedPtr<Fence> pFence)
	{
		VkCommandBuffer commandBuffers[32];
		for (byte i = 0; i < amount; i++)
			commandBuffers[i] = std::static_pointer_cast<VulkanCommandBuffer>(ppCommandBuffers[i])->GetVkCommandBuffer();

		VkSemaphore waitSemaphores[32];
		for (byte i = 0; i < waitSemaphoreCount; i++)
			waitSemaphores[i] = std::static_pointer_cast<VulkanSemaphore>(ppWaitSemaphores[i])->GetVkSemaphore();

		VkSemaphore signalSemaphores[32];
		for (byte i = 0; i < signalSemaphoreCount; i++)
			signalSemaphores[i] = std::static_pointer_cast<VulkanSemaphore>(ppSignalSemaphores[i])->GetVkSemaphore();

		VkPipelineStageFlags waitStages[32];
		for (byte i = 0; i < waitSemaphoreCount; i++)
			waitStages[i] = VulkanPipelineUtils::GetVkPipelineStageFlags(stageFlags[i]);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = amount;
		submitInfo.pCommandBuffers = commandBuffers;
		submitInfo.waitSemaphoreCount = waitSemaphoreCount;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.signalSemaphoreCount = signalSemaphoreCount;
		submitInfo.pSignalSemaphores = signalSemaphores;

		VkFence fence = VK_NULL_HANDLE;
		if (pFence != nullptr)
			fence = std::static_pointer_cast<VulkanFence>(pFence)->GetVkFence();

		CORE_ASSERT(vkQueueSubmit(std::static_pointer_cast<VulkanQueue>(pQueue)->GetVkQueue(), 1, &submitInfo, fence) == VK_SUCCESS, "VulkanDevice", "Failed to submit to queue");
	}

	void VulkanDevice::CopyDescriptorSetImpl(SharedPtr<DescriptorSet> pSrcSet, SharedPtr<DescriptorSet> pDstSet, DescriptorSetCopyDesc& desc)
	{
		auto pDst = std::static_pointer_cast<VulkanDescriptorSet>(pDstSet);
		auto pSrc = std::static_pointer_cast<VulkanDescriptorSet>(pSrcSet);

		VkCopyDescriptorSet infos[32];

		for (uint8 i = 0; i < desc.Entries.size(); i++)
		{
			VkCopyDescriptorSet info = {};
			info.sType = VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET;
			info.srcSet = pSrc->GetVkDescriptorSet();
			info.dstSet = pDst->GetVkDescriptorSet();
			info.srcBinding = desc.Entries[i].SourceBinding;
			info.dstBinding = desc.Entries[i].DestinationBinding;
			info.srcArrayElement = desc.Entries[i].SourceArrayElement;
			info.dstArrayElement = desc.Entries[i].DestinationArrayElement;
			info.descriptorCount = desc.Entries[i].DescriptorCount;

			info.pNext = nullptr;

			infos[i] = info;
		}

		vkUpdateDescriptorSets(mVkDevice, 0, nullptr, desc.Entries.size(), infos);
	}

	void VulkanDevice::UpdateDescriptorSetImpl(SharedPtr<DescriptorSet> pDstSet, DescriptorSetUpdateDesc& desc)
	{
		auto pDst = std::static_pointer_cast<VulkanDescriptorSet>(pDstSet);

		VkWriteDescriptorSet writeInformations[32];
		VkDescriptorBufferInfo writeBufferInformations[32];
		VkDescriptorImageInfo writeImageInformations[32];
		uint32 bufferIndex = 0;
		uint32 imageIndex = 0;

		for (uint8 i = 0; i < desc.Entries.size(); i++)
		{
			VkWriteDescriptorSet writeInfo = {};
			writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo.dstSet = pDst->GetVkDescriptorSet();
			writeInfo.dstBinding = desc.Entries[i].Binding;
			writeInfo.dstArrayElement = desc.Entries[i].ArrayElement;
			writeInfo.descriptorCount = desc.Entries[i].Count;
			writeInfo.descriptorType = VulkanDescriptorUtils::GetVkDescriptorType(desc.Entries[i].Type);
			writeInfo.pNext = nullptr;

			switch (desc.Entries[i].Type)
			{
			case DescriptorType::Sampler:
			{
				auto pSampler = std::static_pointer_cast<VulkanSampler>(desc.Entries[i].pResource);

				VkDescriptorImageInfo samplerInfo = {};
				samplerInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				samplerInfo.imageView = VK_NULL_HANDLE;
				samplerInfo.sampler = pSampler->GetVkSampler();
				writeImageInformations[imageIndex] = samplerInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				imageIndex++;
				break;
			}
			case DescriptorType::CombinedImageSampler:
			{
				// Add code for handling CombinedImageSampler descriptor type
				break;
			}
			case DescriptorType::SampledImage:
			{
				auto pView = std::static_pointer_cast<VulkanTextureBuffer>(desc.Entries[i].pResource);

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = pView->GetVkTextureBuffer();
				imageInfo.sampler = VK_NULL_HANDLE;
				writeImageInformations[imageIndex] = imageInfo;
				writeInfo.pImageInfo = &writeImageInformations[imageIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				imageIndex++;
				break;
			}
			case DescriptorType::StorageImage:
			{
				// Add code for handling StorageImage descriptor type
				break;
			}
			case DescriptorType::UniformTexelBuffer:
			{
				// Add code for handling UniformTexelBuffer descriptor type
				break;
			}
			case DescriptorType::StorageTexelBuffer:
			{
				// Add code for handling StorageTexelBuffer descriptor type
				break;
			}
			case DescriptorType::UniformBuffer:
			{
				auto pBuffer = std::static_pointer_cast<VulkanBuffer>(desc.Entries[i].pResource);

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = pBuffer->GetVkBuffer();
				bufferInfo.offset = desc.Entries[i].BufferOffset;
				bufferInfo.range = pBuffer->GetTotalSizeInBytes();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				bufferIndex++;
				break;
			}
			case DescriptorType::StorageBuffer:
			{
				auto pBuffer = std::static_pointer_cast<VulkanBuffer>(desc.Entries[i].pResource);

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = pBuffer->GetVkBuffer();
				bufferInfo.offset = desc.Entries[i].BufferOffset;
				bufferInfo.range = pBuffer->GetTotalSizeInBytes();
				writeBufferInformations[bufferIndex] = bufferInfo;
				writeInfo.pBufferInfo = &writeBufferInformations[bufferIndex];
				writeInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
				bufferIndex++;
				break;
			}
			case DescriptorType::UniformBufferDynamic:
			{
				// Add code for handling UniformBufferDynamic descriptor type
				break;
			}
			case DescriptorType::StorageBufferDynamic:
			{
				// Add code for handling StorageBufferDynamic descriptor type
				break;
			}
			case DescriptorType::InputAttachment:
			{
				// Add code for handling InputAttachment descriptor type
				break;
			}
			default:
				CORE_LOG(HE_WARNING, "VulkanDevice", "The DescriptorType that you are using is not valid for UpdateDescriptorSet()");
				break;
			}

			writeInformations[i] = writeInfo;
		}

		vkUpdateDescriptorSets(mVkDevice, desc.Entries.size(), writeInformations, 0, nullptr);
	}
}
